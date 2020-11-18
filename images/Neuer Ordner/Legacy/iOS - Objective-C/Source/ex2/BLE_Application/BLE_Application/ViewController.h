//
//  ViewController.h
//  BLE_Application
//
//  Created by Matchbox on 18/02/2014.
//  Copyright (c) 2014 Matchbox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BLEAdapter.h"

@interface ViewController : UIViewController<UITableViewDelegate, UITableViewDataSource>

@property (strong, nonatomic) BLEAdapter* bleWrapper;
@property (strong, nonatomic) CBPeripheral* selected;

@property (weak, nonatomic) IBOutlet UIButton *btn_scan;
- (IBAction)scan_click:(id)sender;

@property (weak, nonatomic) IBOutlet UITableView *tbl_data;

@end
