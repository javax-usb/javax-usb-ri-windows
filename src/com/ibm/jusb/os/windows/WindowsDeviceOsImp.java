package com.ibm.jusb.os.windows;

/*
 * Copyright (c) 2003 Dan Streetman (ddstreet@ieee.org)
 * Copyright (c) 2003 International Business Machines Corporation
 * All Rights Reserved.
 *
 * This software is provided and licensed under the terms and conditions
 * of the Common Public License:
 * http://oss.software.ibm.com/developerworks/opensource/CPLv1.0.htm
 */

import java.util.*;

import javax.usb.*;

import com.ibm.jusb.*;
import com.ibm.jusb.os.*;

/**
 * UsbDeviceOsImp implemenation for Windows platform.
 * <p>
 * This must be set up before use.
 * <ul>
 * <li>The {@link #getUsbDeviceImp() UsbDeviceImp} must be set
 *     either in the constructor or by its {@link #setUsbDeviceImp(UsbDeviceImp) setter}.</li>
 * </ul>
 * @author Dan Streetman
 */
class WindowsDeviceOsImp extends DefaultUsbDeviceOsImp implements UsbDeviceOsImp
{
	/** Constructor */
	public WindowsDeviceOsImp( UsbDeviceImp device )
	{
		setUsbDeviceImp(device);
	}

	/** @return The UsbDeviceImp for this */
	public UsbDeviceImp getUsbDeviceImp() { return usbDeviceImp; }

	/** @param device The UsbDeviceImp for this */
	public void setUsbDeviceImp( UsbDeviceImp device ) { usbDeviceImp = device; }

	/** AsyncSubmit a UsbControlIrpImp */
/*
	public void asyncSubmit( UsbControlIrpImp usbControlIrpImp ) throws UsbException
	{
		WindowsControlRequest request = null;

		if (usbControlIrpImp.isSetConfiguration())
			request = new WindowsSetConfigurationRequest();
		else if (usbControlIrpImp.isSetInterface())
			request = new WindowsSetInterfaceRequest();
		else
			request = new WindowsControlRequest();

		request.setUsbIrpImp(usbControlIrpImp);

		submit(request);
	}
*/

	private UsbDeviceImp usbDeviceImp = null;

}
