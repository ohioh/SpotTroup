using System;
using System.Diagnostics;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Devices.Bluetooth;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Storage.Streams;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Windows.Foundation;
using Windows.Phone.UI.Input;
using System.Threading.Tasks;
using Windows.UI.Xaml.Shapes;
using Windows.UI;
using Windows.UI.Xaml.Media;

namespace ex1
{
    public sealed partial class PeripheralControlPage : Page
    {
        Guid LINKLOSSSERVICEGUID = new Guid("00001803-0000-1000-8000-00805f9b34fb");
        Guid IMMEDIATEALERTSERVICEGUID = new Guid("00001802-0000-1000-8000-00805f9b34fb");
        Guid ALERTLEVELCHARACTERISTICGUID = new Guid("00002a06-0000-1000-8000-00805f9b34fb");

        // custom service which we use to send the current time as HH MM SS to the Arduino Bluetooth Smart device
        Guid TIMEMONITORINGSERVICEUUID = new Guid("3e099912-293f-11e4-93bd-afd0fe6d1dfd");
        Guid CLIENTTIMECHARACTERISTICUUID = new Guid("3e099913-293f-11e4-93bd-afd0fe6d1dfd");

        GattDeviceService time_monitoring_service;

        BluetoothLEDevice mDevice = null;
        Boolean mPlaySoundOnDisconnect = false;
        CoreDispatcher dispatcher = null;
        byte alert_level = 0;

        System.Threading.Timer TimerItem;

        public PeripheralControlPage()
        {
            this.InitializeComponent();
            dispatcher = Window.Current.Dispatcher;
            Windows.Phone.UI.Input.HardwareButtons.BackPressed += HardwareButtons_BackPressed;
            this.NavigationCacheMode = NavigationCacheMode.Disabled;
        }

        protected async override void OnNavigatedTo(NavigationEventArgs e)
        {
            mDevice = await BluetoothLEDevice.FromIdAsync(e.Parameter.ToString());
            if (mDevice != null)
            {
                // update connect text in the UI 
                connectionStatus.Text = mDevice.ConnectionStatus.ToString();

                if (mDevice.ConnectionStatus == BluetoothConnectionStatus.Disconnected)
                {
                    btn_low.IsEnabled = false;
                    btn_mid.IsEnabled = false;
                    btn_high.IsEnabled = false;
                    btn_noise.IsEnabled = false;
                    share_mode.IsEnabled = false;
                    return;
                }

                initialiseGattResources();

            }

            DateTime now = DateTime.Now;
            string date_patt = @"HH:mm:ss";
            string hhmmss = now.ToString(date_patt);
            time_now.Text = hhmmss;

            // Start timer task to collect (and if necessary share using Bluetooth GATT) the system time
            System.Threading.TimerCallback TimerDelegate = new System.Threading.TimerCallback(TimerTask);
            TimerItem = new System.Threading.Timer(TimerDelegate, null, 1000, 1000);
        }

        private async void initialiseGattResources()
        {
            // hook up the connection status change callback 
            mDevice.ConnectionStatusChanged += ConnectionChanged;

            // get the link loss service 
            GattDeviceService service = mDevice.GetGattService(LINKLOSSSERVICEGUID);
            if (service != null)
            {
                // get the characteristic 
                GattCharacteristic characteristic = service.GetCharacteristics(ALERTLEVELCHARACTERISTICGUID)[0];
                if (characteristic != null)
                {
                    // read its current value 
                    GattReadResult readValue = await characteristic.ReadValueAsync();
                    UpdateAlertLevel(Windows.Storage.Streams.DataReader.FromBuffer(readValue.Value).ReadByte());
                }
            }
            // get the time monitoring service 
            time_monitoring_service = mDevice.GetGattService(TIMEMONITORINGSERVICEUUID);

        }

        private void HardwareButtons_BackPressed(object sender, BackPressedEventArgs e)
        {
            Debug.WriteLine("HardwareButtons_BackPressed....");
            if (this.Frame.CanGoBack)
            {
                e.Handled = true;
                this.Frame.GoBack();
            }
        }
        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            Debug.WriteLine("OnNavigatingFrom....");
            if (TimerItem != null)
            {
                TimerItem.Dispose();
                Debug.WriteLine("Disposed of timer....");
            }
            if (share_mode.IsChecked == true)
            {
                WriteGattClientTime((byte)00, (byte)00, (byte)00);
            }
            Debug.WriteLine("Switched off LEDs....");
        }

        private async void TimerTask(object obj)
        {
            DateTime now = DateTime.Now;
            string date_patt = @"HH:mm:ss";
            string hhmmss = now.ToString(date_patt);
            int hh = now.Hour;
            int mm = now.Minute;
            int ss = now.Second;
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                time_now.Text = hhmmss;
                if (share_mode.IsChecked == true)
                {
                    WriteGattClientTime(hh, mm, ss);
                }
            });
        }

        private void WriteGattClientTime(int hh, int mm, int ss)
        {
            Debug.WriteLine("Sharing time with Bluetooth GATT Time Monitoring service....");
            if (time_monitoring_service != null)
            {
                // get the characteristic 
                var characteristic = time_monitoring_service.GetCharacteristics(CLIENTTIMECHARACTERISTICUUID)[0];
                if (characteristic != null)
                {
                    // create a data writer to write the value 
                    DataWriter writer = new DataWriter();
                    writer.WriteByte((byte)hh);
                    writer.WriteByte((byte)mm);
                    writer.WriteByte((byte)ss);
                    if (ss >= 0)
                    {
                        ColorRectangle(Q1, Colors.Gray);
                        ColorRectangle(Q2, Colors.Gray);
                        ColorRectangle(Q3, Colors.Gray);
                    }
                    if (ss >= 15)
                    {
                        ColorRectangle(Q1, Colors.Green);
                    }
                    if (ss >= 30)
                    {
                        ColorRectangle(Q2, Colors.Yellow);
                    }
                    if (ss >= 45)
                    {
                        ColorRectangle(Q3, Colors.Red);
                    }
                    // Attempt to write the data to the device 
                    characteristic.WriteValueAsync(writer.DetachBuffer(), GattWriteOption.WriteWithoutResponse);
                }
            }
            else
            {
                Debug.WriteLine("ERROR: WriteGattClienTime could not obtain client time characteristic");
                message.Text = "ERROR: WriteGattClienTime could not obtain client time characteristic";
            }
        }

        private void HandleUnchecked(object sender, RoutedEventArgs e)
        {
            // send values to GATT Time Monitoring Service which will cause all LEDs to be switched off
            WriteGattClientTime((byte) 00, (byte) 00, (byte) 00);
            ColorRectangle(Q1, Colors.Black);
            ColorRectangle(Q2, Colors.Black);
            ColorRectangle(Q3, Colors.Black);
        }

        private void ColorRectangle(Rectangle rectangle, Color color)
        {
            SolidColorBrush solid_color_brush = new SolidColorBrush();
            solid_color_brush.Color = color;
            rectangle.Fill = solid_color_brush;
        }

        private async void ConnectionChanged(BluetoothLEDevice device, object obj)
        {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                connectionStatus.Text = device.ConnectionStatus.ToString();
                if (device.ConnectionStatus == BluetoothConnectionStatus.Disconnected)
                {
                    Debug.WriteLine("ConnectionChanged to Disconnected");
                    btn_low.IsEnabled = false;
                    btn_mid.IsEnabled = false;
                    btn_high.IsEnabled = false;
                    btn_noise.IsEnabled = false;
                    share_mode.IsEnabled = false;
                }
                else
                {
                    Debug.WriteLine("ConnectionChanged to Connected");
                    btn_low.IsEnabled = true;
                    btn_mid.IsEnabled = true;
                    btn_high.IsEnabled = true;
                    btn_noise.IsEnabled = true;
                    share_mode.IsEnabled = true;
                }
                if (mPlaySoundOnDisconnect && device.ConnectionStatus == BluetoothConnectionStatus.Disconnected) { mediaPlayer.Play(); }
            });
        }
        private void UpdateAlertLevel(byte value)
        {
            alert_level = value;
            if (value == 2)
            {
                mPlaySoundOnDisconnect = true;
            }
            else
            {
                mPlaySoundOnDisconnect = false;
            }
            alertLevel.Text = "Alert Level = " + value.ToString();
        }

        private void LowButton_Click(object sender, RoutedEventArgs e) { SetLevel(0); }
        private void MidButton_Click(object sender, RoutedEventArgs e) { SetLevel(1); }
        private void HighButton_Click(object sender, RoutedEventArgs e) { SetLevel(2); }
        private async void MakeNoiseButton_Click(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("MakeNoiseButton_Click");
            // get the immediate alert service 
            try
            {
                GattDeviceService service = mDevice.GetGattService(IMMEDIATEALERTSERVICEGUID);
                if (service != null)
                {
                    Debug.WriteLine("MakeNoiseButton_Click found GattDeviceService");
                    // get the alert level characteristic 
                    var characteristic = service.GetCharacteristics(ALERTLEVELCHARACTERISTICGUID)[0];
                    if (characteristic != null)
                    {
                        Debug.WriteLine("MakeNoiseButton_Click found alert level characteristic from immediate alert service");
                        // create a data writer to write the value 
                        DataWriter writer = new DataWriter();
                        writer.WriteByte(alert_level);
                        // Attempt to write the data to the device 
                        Debug.WriteLine("MakeNoiseButton_Click writing value of {0} to characteristic", alert_level);
                        GattCommunicationStatus status = await characteristic.WriteValueAsync(writer.DetachBuffer(), GattWriteOption.WriteWithoutResponse);
                        Debug.WriteLine("MakeNoiseButton_Click done writing value to characteristic");
                    }
                }
                else
                {
                    Debug.WriteLine("MakeNoiseButton_Click did NOT find GattDeviceService");
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("MakeNoiseButton_Click: exception {0}:{1}", ex.GetType(), ex.Message);
            }

        }

        private async void SetLevel(byte value)
        {
            if (mDevice == null)
            {
                Debug.WriteLine("SetLevel: No device available - ignoring request");
                return;
            }
            // get the link loss service 
            GattDeviceService service = mDevice.GetGattService(LINKLOSSSERVICEGUID);
            if (service != null)
            {
                // get the characteristic 
                var characteristic = service.GetCharacteristics(ALERTLEVELCHARACTERISTICGUID)[0];
                if (characteristic != null)
                {
                    // create a data writer to write the value 
                    DataWriter writer = new DataWriter();
                    writer.WriteByte(value);
                    // Attempt to write the data to the device 
                    // and whist doing so get the status. 
                    GattCommunicationStatus status = await characteristic.WriteValueAsync(writer.DetachBuffer());
                    if (status == GattCommunicationStatus.Success)
                    {
                        UpdateAlertLevel(value);
                    }
                }
            }
        }
    }
}
