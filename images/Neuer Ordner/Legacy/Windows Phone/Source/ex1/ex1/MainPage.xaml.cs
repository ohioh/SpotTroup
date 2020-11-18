using System;
using System.Collections.Generic;
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

    public sealed partial class MainPage : Page
    {
        Guid LINKLOSSSERVICEGUID = new Guid("00001803-0000-1000-8000-00805f9b34fb");
        DeviceInformationCollection foundDevices = null;

        public MainPage()
        {
            this.InitializeComponent();
            this.NavigationCacheMode = NavigationCacheMode.Required;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }

        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
        }

        private async void ScanButton_Click(object sender, RoutedEventArgs e)
        {
            foundDevices = await DeviceInformation.FindAllAsync(GattDeviceService.GetDeviceSelectorFromUuid
 (LINKLOSSSERVICEGUID));
            int device_count = foundDevices.Count;
            deviceList.Items.Clear();
            for (int i = 0; i < foundDevices.Count; i++)
            {
                DeviceInformation deviceInfo = foundDevices[i];
                deviceList.Items.Add(deviceInfo.Name);
            }
        }

    }
}
