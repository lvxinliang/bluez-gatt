#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <errno.h>

#include "device_adv.h"
#include "lib/bluetooth.h"
#include "lib/hci.h"
#include "lib/hci_lib.h"
int app_hci_le_adv()
{
    int hdev = -1;
    int dd = -1;
    struct hci_request rq;
    le_set_advertise_enable_cp advertise_cp;
    le_set_advertising_parameters_cp adv_params_cp;
    uint8_t status;
    int ret;

    if (hdev < 0)
        hdev = hci_get_route(NULL);

    dd = hci_open_dev(hdev);
    if (dd < 0)
    {
        printf("could not open device\n");
        return -1;
    }

    /* Set advertising parameter */
    memset(&adv_params_cp, 0, sizeof(adv_params_cp));
    adv_params_cp.min_interval = htobs(0x0030);
    adv_params_cp.max_interval = htobs(0x0030);
    adv_params_cp.advtype = 0x00; /* 0: public, 1: random*/
    adv_params_cp.chan_map = 7;

    memset(&rq, 0, sizeof(rq));
    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_ADVERTISING_PARAMETERS;
    rq.cparam = &adv_params_cp;
    rq.clen = LE_SET_ADVERTISING_PARAMETERS_CP_SIZE;
    rq.rparam = &status;
    rq.rlen = 1;

    ret = hci_send_req(dd, &rq, 1000);
    if (ret < 0)
    {
        printf("hci_send_req Failed\n");
        goto done;
    }

    /* Enable advertising */
    memset(&advertise_cp, 0, sizeof(advertise_cp));
    advertise_cp.enable = 0x01;

    memset(&rq, 0, sizeof(rq));
    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_ADVERTISE_ENABLE;
    rq.cparam = &advertise_cp;
    rq.clen = LE_SET_ADVERTISE_ENABLE_CP_SIZE;
    rq.rparam = &status;
    rq.rlen = 1;

    ret = hci_send_req(dd, &rq, 1000);

done:
    hci_close_dev(dd);

    if (ret < 0)
    {
        printf("Can't set advertise mode on hci%d: %s (%d)\n",
               hdev, strerror(errno), errno);
        return -1;
    }

    if (status)
    {
        printf("LE set advertise enable on hci%d returned status %d\n",
               hdev, status);
        return -1;
    }
    return 0;
}

/* Disable LE advertising */
int app_hci_no_le_adv()
{
    int hdev = -1;
    int dev_fd = -1;
    struct hci_request rq;
    le_set_advertise_enable_cp advertise_cp;
    uint8_t status;
    int ret;

    if (hdev < 0)
        hdev = hci_get_route(NULL);

    dev_fd = hci_open_dev(hdev);
    if (dev_fd < 0)
    {
        printf("dev_fd: %d is not available\n", dev_fd);
        return -1;
    }

    memset(&advertise_cp, 0, sizeof(advertise_cp));

    memset(&rq, 0, sizeof(rq));
    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_ADVERTISE_ENABLE;
    rq.cparam = &advertise_cp;
    rq.clen = LE_SET_ADVERTISE_ENABLE_CP_SIZE;
    rq.rparam = &status;
    rq.rlen = 1;

    ret = hci_send_req(dev_fd, &rq, 1000);

    hci_close_dev(dev_fd);
    if (12 == status)
    {
        return 0;
    }
    if (ret < 0)
    {
        printf("Can't set advertise mode on hci%d: %s (%d)\n",
               hdev, strerror(errno), errno);
        return -1;
    }

    if (status)
    {
        printf("LE set advertise enable on hci%d returned status %d\n",
               hdev, status);
        return -1;
    }

    return 0;
}

static void fill_scan_response_data(le_set_scan_response_data_cp *response_cp)
{
    char response_data[] = {
        0x8f,
        0x03,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07};
    response_cp->data[0] = 0x0a; /* Field Length */
    response_cp->data[1] = 0xff; /* Field Type */
    memcpy(response_cp->data + 2, response_data, sizeof(response_data));
}

/* Set scan response data */
static int app_hci_le_set_scan_response_data(int dd, int hdev)
{
    struct hci_request rq;
    le_set_scan_response_data_cp response_cp;
    uint8_t status;
    int ret;

    if (hdev < 0)
        hdev = hci_get_route(NULL);

    memset(&response_cp, 0, sizeof(response_cp));
    memset(&rq, 0, sizeof(rq));

    fill_scan_response_data(&response_cp);
    response_cp.length = 1 + response_cp.data[0];

    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_SCAN_RESPONSE_DATA;
    rq.cparam = &response_cp;
    rq.clen = LE_SET_SCAN_RESPONSE_DATA_CP_SIZE;
    rq.rparam = &status;
    rq.rlen = 1;

    ret = hci_send_req(dd, &rq, 1000);

    if (ret < 0)
    {
        printf("Can't set response data on hci%d: %s (%d)\n",
               hdev, strerror(errno), errno);
        return -1;
    }

    if (status)
    {
        printf("LE set response data on hci%d failed, returned status %d\n",
               hdev, status);
        return -1;
    }

    return 0;
}

int app_hci_le_set_advertising_data()
{
    int dd = -1;
    int hdev = -1;
    int ret;
    struct hci_request rq;
    le_set_advertising_data_cp advertise_cp;
    uint8_t status;

    if (hdev < 0)
        hdev = hci_get_route(NULL);

    dd = hci_open_dev(hdev);
    if (dd < 0)
    {
        printf("dev_fd: %d is not available\n", dd);
        return -1;
    }
    memset(&advertise_cp, 0, sizeof(advertise_cp));

    /* Adv Flags */
    advertise_cp.data[0] = 0x02; /* Field Length */
    advertise_cp.data[1] = 0x01; /* Flags Type */
    //advertise_cp.data[2] = 0x02;          /* LE General Discoverable Mode */
    //advertise_cp.data[2] |= 0x04;         /* BR/EDR Not Supported */
    advertise_cp.data[2] = 0x1a;

    /* Adv Name */
    advertise_cp.data[3] = 1 + strlen(DEVICE_NAME);                  /* Field length */
    advertise_cp.data[4] = 0x09;                                     /* Name Type */
    memcpy(advertise_cp.data + 5, DEVICE_NAME, strlen(DEVICE_NAME)); /* Name Data */

    /* data len */
    advertise_cp.length = 1 + advertise_cp.data[0] + 1 + advertise_cp.data[3];

    printf("adv name: %s, len: %d\n", DEVICE_NAME, strlen(DEVICE_NAME));

    memset(&rq, 0, sizeof(rq));
    rq.ogf = OGF_LE_CTL;
    rq.ocf = OCF_LE_SET_ADVERTISING_DATA;
    rq.cparam = &advertise_cp;
    rq.clen = LE_SET_ADVERTISING_DATA_CP_SIZE;
    rq.rparam = &status;
    rq.rlen = 1;

    ret = hci_send_req(dd, &rq, 1000);

    app_hci_le_set_scan_response_data(dd, hdev);

    hci_close_dev(dd);
    if (ret < 0)
    {
        printf("Can't set advertise data on hci%d: %s (%d)\n",
               hdev, strerror(errno), errno);
        return -1;
    }

    if (status)
    {
        printf("LE set advertise data on hci%d returned status %d\n",
               hdev, status);
        return -1;
    }

    return 0;
}

int start_provision_adv()
{
    int code = 0;
    code = app_hci_no_le_adv();
    if (code)
    {
        goto fail;
    }
    code = app_hci_le_set_advertising_data();
    if (code)
    {
        goto fail;
    }
    code = app_hci_le_adv();
fail:
    return code;
}