#include <stdio.h>
#include <wchar.h>
#include <hidapi/hidapi.h>

int main() {
    if (hid_init())
    {
        printf("Failed to initialize HIDAPI\n");
        return 1;
    }

    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev) {
        printf("Device Found:\n");
        printf("  Vendor ID : 0x%04hx\n", cur_dev->vendor_id);
        printf("  Product ID: 0x%04hx\n", cur_dev->product_id);
        printf("  Serial No.: %ls\n", cur_dev->serial_number);
        printf("  Product   : %ls\n", cur_dev->product_string);
        printf("  Manufacturer: %ls\n\n", cur_dev->manufacturer_string);

        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
    hid_exit();

    return 0;
}
