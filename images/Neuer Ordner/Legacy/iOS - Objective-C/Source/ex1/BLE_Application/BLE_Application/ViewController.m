//
//  ViewController.m
//  BLE_Application
//
//  Created by Matchbox on 18/02/2014.
//  Updated on 10/25/2014 to accomodate the service change on Arduino
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize bleWrapper;
NSTimer* scannerTimer;

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
    
    // call the scan function, specifiying a 2 second scan time.
    [self.bleWrapper findBLEPeripherals:2 serviceUUID:kServiceUUID];
    
    // set progress spinner showing we are loading something
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    
    // Initiate timer to go off in 2 seconds
    [NSTimer scheduledTimerWithTimeInterval:(float)2.0 target:self selector:@selector(connectionTimer:) userInfo:nil repeats:NO];
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
    return [self.bleWrapper.peripherals count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if(cell == nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    CBPeripheral * temp = (CBPeripheral *)[self.bleWrapper.peripherals objectAtIndex:indexPath.row];
    cell.textLabel.text = temp.name;
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    CBPeripheral *temp =(CBPeripheral *)[self.bleWrapper.peripherals objectAtIndex:indexPath.row];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Result" message:temp.name delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles: nil];
    [alert show];
}
@end
