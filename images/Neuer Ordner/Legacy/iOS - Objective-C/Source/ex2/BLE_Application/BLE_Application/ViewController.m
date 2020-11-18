//
//  ViewController.m
//  BLE_Application
//
//  Created by Matchbox on 18/02/2014.
//  Copyright (c) 2014 Matchbox. All rights reserved.
//

#import "ViewController.h"
#import "PeripheralViewController.h"
#import "BLEAdapter.h"


@interface ViewController ()

@end

@implementation ViewController

@synthesize bleWrapper;
@synthesize selected;
NSTimer* scannerTimer;

//Link loss service
static NSString * const kServiceUUID = @"1803";

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    self.bleWrapper = [BLEAdapter sharedBLEAdapter];
    [self.bleWrapper controlSetup:1];
    
    [self.tbl_data setDelegate:self];
    [self.tbl_data setDataSource: self];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)scan_click:(id)sender
{
    // Stop button from being able to be clicked again untilscan is finished
    [self.btn_scan setEnabled:false];
    
    // call the scan function, specifiying a 5 second scan time.
    [self.bleWrapper findBLEPeripherals:5 serviceUUID:kServiceUUID];
    
    // set progress spinner showing we are loading something
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    
    // Initiate timer to go off in 5 seconds
    [NSTimer scheduledTimerWithTimeInterval:(float)5.0 target:self selector:@selector(connectionTimer:) userInfo:nil repeats:NO];
}

// Called when scan period is over
-(void) connectionTimer:(NSTimer *)timer
{
    // stop progress spinner
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    
    NSString *message;
    
    // Display alert to show if we have found devices
    if ( self.bleWrapper.peripherals.count > 0)
    {
        NSLog(@"RELOADING DATA");
        [self.tbl_data reloadData];
    }
    else
    {
       message = @"We have not found devices";
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Result" message:message delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles: nil];
        [alert show];
    }
    
    // enable scan button
    [self.btn_scan setEnabled:true];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // define how many cells we need for the table
    return [self.bleWrapper.peripherals count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    // get re usable cell object
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if(cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    // get a peripheral object, set the text value to the peripheral name.
    CBPeripheral * temp = (CBPeripheral *)[self.bleWrapper.peripherals objectAtIndex:indexPath.row];
    cell.textLabel.text = temp.name;
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    CBPeripheral *temp =(CBPeripheral *)[self.bleWrapper.peripherals objectAtIndex:indexPath.row];
    self.selected = temp;
    [self performSegueWithIdentifier:@"ScanToPeripheral" sender:self];
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if([segue.identifier isEqualToString:@"ScanToPeripheral"])
    {
        PeripheralViewController *destViewController = segue.destinationViewController;
        [destViewController setData: self.bleWrapper : self.selected];
    }
}

@end
