#ifndef __DEVICE_ADV__
#define __DEVICE_ADV__
#define DEVICE_NAME "lxl_raspberry"

int app_hci_le_adv();

int app_hci_no_le_adv();

int app_hci_le_set_advertising_data();

int start_provision_adv();
#endif