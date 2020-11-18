//
//  PeripheralViewController.m
//  BLE_Application
//
//  Created by Matchbox on 18/02/2014.
//  Copyright (c) 2014 Matchbox. All rights reserved.
//

#import "PeripheralViewController.h"

@interface PeripheralViewController ()

@end

@implementation PeripheralViewController

@synthesize toConnect;
@synthesize bleAdapter;
@synthesize player;

NSTimer* rssiTimer;

NSString *LINK_LOSS_SERVICE_UUID = @"1803";
NSString *IMMEDIATE_ALERT_SERVICE_UUID = @"1802";
NSString *PROXIMITY_MONITORING_SERVICE_UUID = @"3E099910-293F-11E4-93BD-AFD0FE6D1DFD";

NSString *ALERT_LEVEL_CHARACTERISTIC_UUID = @"2A06";
NSString *CLIENT_PROXIMITY_CHARACTERISTIC_UUID = @"3E099911-293F-11E4-93BD-AFD0FE6D1DFD";

CBService *LinkLoss;
CBService *ImmediateAlert;
CBService *ProximityMonitoring;

// Alert Level belonging to the Link Loss Service
CBCharacteristic *LL_AlertLevelCharacteristic;
// Immediate Alert Service
CBCharacteristic *IA_AlertLevelCharacteristic;

CBCharacteristic *ClientProximityCharacteristic;

int CurrentAlertLevel = 2;
bool share_proximity_data = false;

// Set data from a Segue from outside of this view controller
-(void) setData: (BLEAdapter *) adapter : (CBPeripheral *) peripheral
{
    self.bleAdapter = adapter;
    self.toConnect = peripheral;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self.lbl_rssi setHidden:true];
    
    // Set device name to label text
    self.lbl_name.text = self.toConnect.name;
    [self.bleAdapter setDelegate:self];
    
    [self setAsDisconnected];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)connect_click:(id)sender
{
    
    // If the CBPeripheral is not null try to connect to it
    if (self.toConnect != Nil)
    {
        
        [self.bleAdapter connectPeripheral:self.toConnect status:TRUE];
        [self.btn_connect setEnabled:false];
    }
}

// Call back function, which will fire when we know if we are connected or not
-(void) OnConnected:(BOOL)status
{
    if (status)
    {
        
        [self stopSound];
        [self setAsConnected];
        [self.lbl_rssi setHidden:false];
        [bleAdapter getAllServicesFromPeripheral:self.bleAdapter.activePeripheral];
        
        NSLog(@"Starting RSSI timer");
        
        // Do not reduce the time interval below 3 seconds due to battery life considerations, also in our testing below a polling frequency of less that 3 seconds we found issues with the main thread
        rssiTimer = [NSTimer scheduledTimerWithTimeInterval:3.0 target:self selector:@selector(readRSSITimer:) userInfo:nil repeats:YES];
    }
    else
    {
        [self connectionFailed];
    }
    
}


-(void) readRSSITimer:(NSTimer *)timer
{
    NSLog(@"Reading RSSI value");
    
    [self.bleAdapter.activePeripheral readRSSI];
    
    if (self.bleAdapter.activePeripheral.RSSI != NULL)
    {
        self.lbl_rssi.text = [NSString stringWithFormat: @"RSSI : %@", self.bleAdapter.activePeripheral.RSSI];
        
        Byte rssi = [self.bleAdapter.activePeripheral.RSSI integerValue] * -1;
        Byte proximity_band;
        
        if (rssi <= 50)
        {
            proximity_band = 0;
        }
        else if (rssi > 50 && rssi < 80)
        {
            proximity_band = 1;
        }
        else
        {
            proximity_band = 2;
        }
        [self ChangeRSSIColour : proximity_band];
        
        NSLog(@"PROXIMITY BAND : %d  RSSI : %d", proximity_band , rssi);
        
        if (share_proximity_data) {
            char bytes[2];
            bytes[0] = proximity_band + 1;
            bytes[1] = rssi * -1;
            int len = 2;
            NSData* tmpData = [NSData dataWithBytes:bytes length:len];
            [self.bleAdapter writeValueNoResponse :(CBCharacteristic *)ClientProximityCharacteristic  p:(CBPeripheral *)self.bleAdapter.activePeripheral data:(NSData *)tmpData];
        }
    } else {
        NSLog(@"RSSI not available");
    }
    
    if (LL_AlertLevelCharacteristic != NULL)
    {
        int serviceCode = [self.bleAdapter CBUUIDToInt: LinkLoss.UUID];
        int characteristicCode = [self.bleAdapter CBUUIDToInt: LL_AlertLevelCharacteristic.UUID];
        
        [self.bleAdapter readValue:(int)serviceCode characteristicUUID:(int)characteristicCode  p:(CBPeripheral *)self.bleAdapter.activePeripheral];
    }
}

-(void) ChangeRSSIColour: (int) level
{
    switch (level)
    {
        case 1:
            [self.pnl_hotCold setBackgroundColor:[UIColor orangeColor]];
            break;
        case 2:
            [self.pnl_hotCold setBackgroundColor:[UIColor redColor]];
            break;
        default:
            [self.pnl_hotCold setBackgroundColor:[UIColor greenColor]];
            break;
    }
    [self.pnl_hotCold setNeedsDisplay];
}

-(void) OnDiscoverServices:(NSArray *)s
{
    for (CBService *service in s)
    {
        NSLog(@"SERVICE : %s", [self.bleAdapter CBUUIDToString: service.UUID]);
        
        if ([service.UUID isEqual:[CBUUID UUIDWithString: LINK_LOSS_SERVICE_UUID]])
        {
            NSLog(@"Got Link Loss Service");
            LinkLoss = service;
            [self.bleAdapter getAllCharacteristicsForService:self.bleAdapter.activePeripheral service:LinkLoss];
        }
        if ([service.UUID isEqual:[CBUUID UUIDWithString: IMMEDIATE_ALERT_SERVICE_UUID]])
        {
            NSLog(@"Got Immediate Alert Service");
            ImmediateAlert = service;
            [self.bleAdapter getAllCharacteristicsForService:self.bleAdapter.activePeripheral service:ImmediateAlert];
        }
        if ([service.UUID isEqual:[CBUUID UUIDWithString: PROXIMITY_MONITORING_SERVICE_UUID]])
        {
            NSLog(@"Got Proximity Monitoring Service");
            ProximityMonitoring = service;
            [self.bleAdapter getAllCharacteristicsForService:self.bleAdapter.activePeripheral service:ProximityMonitoring];
        }
    }
}

-(void) OnDiscoverCharacteristics:(NSArray *)c
{
    for (CBCharacteristic *character in c)
    {
        if ([character.UUID isEqual:[CBUUID UUIDWithString: ALERT_LEVEL_CHARACTERISTIC_UUID]])
        {
            if ([character.service.UUID isEqual:[CBUUID UUIDWithString: LINK_LOSS_SERVICE_UUID]])
            {
                NSLog(@"Got Link Loss Service's Alert Level characteristic");
                LL_AlertLevelCharacteristic = character;
            } else {
                if ([character.service.UUID isEqual:[CBUUID UUIDWithString: IMMEDIATE_ALERT_SERVICE_UUID]])
                {
                    NSLog(@"Got Immediate Alert Service's Alert Level characteristic");
                    IA_AlertLevelCharacteristic = character;
                }
            }
        }
        else if([character.UUID isEqual:[CBUUID UUIDWithString: CLIENT_PROXIMITY_CHARACTERISTIC_UUID]])
        {
            NSLog(@"Got Client Proximity characteristic");
            ClientProximityCharacteristic = character;
        }
    }
}

// If we do not connect display error
-(void)connectionFailed
{
    [rssiTimer invalidate];
    
    
    if (CurrentAlertLevel == 2)
    {
        [self playSound];
    }

    [self.navigationController popToRootViewControllerAnimated:TRUE];
    [self setAsDisconnected];

}

// Helper method
-(void)displayMessage:(NSString*)message
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"DEBUG" message: message delegate: nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
}

-(void)setAsDisconnected
{
    [self.btn_connect setEnabled: true];
    [self.connection_status setText:@"Status: Disconnected"];
    [self.btn_makeNoise setEnabled: false];
}
-(void)setAsConnected
{
    [self.btn_connect setEnabled: false];
    [self.connection_status setText:@"Status: Connected"];
    [self.btn_makeNoise setEnabled: true];
}

- (IBAction)send_lowAlert:(id)sender
{
    [self changeAlertLevel:0];
}

- (IBAction)send_midAlert:(id)sender
{
    [self changeAlertLevel:1];
}

- (IBAction)send_highAlert:(id)sender
{
    [self changeAlertLevel:2];
}

-(void)changeAlertLevel:(int) level
{
    if (level <= 2 && link != NULL)
    {
        CurrentAlertLevel = level;
        
        char* bytes = (char*) &level;
        int len = 1;
        NSData* tmpData = [NSData dataWithBytes:bytes length:len];
        
        NSLog(@"Link loss UUID is %@", LinkLoss.UUID);
        NSLog(@"Alert Level characteristic UUID is %@", ALERT_LEVEL_CHARACTERISTIC_UUID);
        
        NSLog(@"incoming alert level %D", level);
        
        [self.bleAdapter.activePeripheral writeValue:tmpData forCharacteristic:LL_AlertLevelCharacteristic type:CBCharacteristicWriteWithResponse];
    }
}

-(void) OnWriteDataChanged:(BOOL)status
{
    if (status)
    {
        NSLog(@"Successful Write");
        // if we just updated the alert level then we should display the new current value in the UI... if not it's simplest to do this anyway
        self.lbl_alert.text = [NSString stringWithFormat:@"alert: %d", CurrentAlertLevel];
    }else{
        NSLog(@"Error Writing value");
    }
}

-(void) OnReadDataChanged:(BOOL)status : (CBCharacteristic*) characteristic
{
    if (status)
    {
        NSLog(@"Successful Read %@", characteristic.value);
        self.lbl_alert.text = [NSString stringWithFormat:@"alert: %@", characteristic.value];
    }else{
        NSLog(@"Error Reading value");
    }
}

- (IBAction)noise_click:(id)sender
{
    
    NSLog(@"Entered noise_click");
    
    if (IA_AlertLevelCharacteristic != NULL)
    {
        
        char bytes[1];
        Byte val = CurrentAlertLevel;
        bytes[0] = val;
        int len = 1;
        NSData* tmpData = [NSData dataWithBytes:bytes length:len];
        
        [self.bleAdapter writeValueNoResponse :(CBCharacteristic *)IA_AlertLevelCharacteristic  p:(CBPeripheral *)self.bleAdapter.activePeripheral data:(NSData *)tmpData];
        
        NSLog(@"Requesting alert_level written to Immediate Alert Service's Alert Level without response");
        
    }
}

- (IBAction)share_switch_changed:(id)sender
{
    NSLog(@"Entered share_switch_changed");
    
    UISwitch *mySwitch = (UISwitch *)sender;
    if ([mySwitch isOn]) {
        NSLog(@"Proximity Sharing Enabled!");
        share_proximity_data = true;
    } else {
        NSLog(@"Proximity Sharing Disabled");
        share_proximity_data = false;
        // tell the arduino to switch off LEDS and clear LCD display
        char bytes[2];
        bytes[0] = 0;
        bytes[1] = 0;
        int len = 2;
        NSData* tmpData = [NSData dataWithBytes:bytes length:len];
        [self.bleAdapter writeValueNoResponse :(CBCharacteristic *)ClientProximityCharacteristic  p:(CBPeripheral *)self.bleAdapter.activePeripheral data:(NSData *)tmpData];
        
    }
    
}


-(void) playSound
{
    NSString *soundFilePath = [[NSBundle mainBundle] pathForResource:@"Beep" ofType:@"wav"];
    NSURL *soundFileURL = [NSURL fileURLWithPath:soundFilePath];
    player = [[AVAudioPlayer alloc] initWithContentsOfURL:soundFileURL error:nil];
    player.numberOfLoops = -1; //infinite
    [player play];
}

-(void) stopSound
{
    [player stop];
}
@end