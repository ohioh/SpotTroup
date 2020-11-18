//
//  PeripheralViewController.h
//  BLE_Application
//
//  Created by Matchbox on 18/02/2014.
//  Copyright (c) 2014 Matchbox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BLEAdapter.h"

#import <AVFoundation/AVAudioPlayer.h>
#import <AVFoundation/AVFoundation.h>

@interface PeripheralViewController : UIViewController

@property (weak, nonatomic) IBOutlet UILabel *lbl_name;

- (void) setData: (BLEAdapter *) adapter : (CBPeripheral *) peripheral;

@property (strong, nonatomic) BLEAdapter *bleAdapter;
@property (strong, nonatomic) CBPeripheral *toConnect;

@property (weak, nonatomic) IBOutlet UIButton *btn_connect;
- (IBAction)connect_click:(id)sender;

@property (weak, nonatomic) IBOutlet UILabel *lbl_rssi;
@property (weak, nonatomic) IBOutlet UIView *pnl_hotCold;

- (IBAction)send_lowAlert:(id)sender;
- (IBAction)send_midAlert:(id)sender;
- (IBAction)send_highAlert:(id)sender;

@property (weak, nonatomic) IBOutlet UIButton *btn_makeNoise;
- (IBAction)noise_click:(id)sender;

- (IBAction)share_switch_changed:(id)sender;

@property (strong, nonatomic) AVAudioPlayer *player;

@property (weak, nonatomic) IBOutlet UILabel *lbl_alert;
@property (weak, nonatomic) IBOutlet UILabel *connection_status;

@end
