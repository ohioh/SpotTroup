using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Devices.Bluetooth.GenericAttributeProfile;
using Windows.Devices.Enumeration;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=391641

namespace ex1
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {

        Guid LINKLOSSSERVICEGUID = new Guid("00001803-0000-1000-8000-00805f9b34fb");
        DeviceInformationCollection foundDevices = null;

        public MainPage()
        {
            this.InitializeComponent();
            this.NavigationCacheMode = NavigationCacheMode.Required;
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            Debug.WriteLine("MainPage::OnNavigatedTo....");
            deviceList.Items.Clear();
        }


        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e) {

            var lv = sender as ListView;
            if (lv != null)
            {
                if (lv.SelectedIndex >= 0 && lv.SelectedIndex < foundDevices.Count)
                {
                    DeviceInformation devInfo = foundDevices[lv.SelectedIndex];
                    this.Frame.Navigate(typeof(PeripheralControlPage), devInfo.Id);
                }
            }

        }

        private async void ScanButton_Click(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("ScanButton_Click");
            foundDevices = await DeviceInformation.FindAllAsync(GattDeviceService.GetDeviceSelectorFromUuid(LINKLOSSSERVICEGUID));
            int device_count = foundDevices.Count;
            Debug.WriteLine("Found {0} devices", device_count);
            deviceList.Items.Clear();
            for (int i = 0; i < foundDevices.Count; i++)
            { 
                DeviceInformation deviceInfo = foundDevices[i]; 
                deviceList.Items.Add(deviceInfo.Name); 
            }
        }
    }

}
