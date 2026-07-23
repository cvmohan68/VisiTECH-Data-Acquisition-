# VisiTECH-Data-Acquisition
This Data Acquisition package communicates with any MODBUS complaint devices
(Such as PLC's, SLPC and PID controllers, etc.,), and any OPC DA Servers. With the
OPC Client interface you can also acquire the data from any SCADA, DCS systems that
exposes the OPC DA servers (Most SCADA-systems today act as OPC servers).
System Requirements:
• Minimum Intel’s Pentium-II class processor running with 350MHz or above.
• RAM 64 MB or above.
• Hard disk space 20 MB minimum.
• VGA Monitor with 1024 X 768 resolutions only.
• Operating Systems Windows NT, 2000, XP or later version.
Some of the features of DA package:
1. At present this package supports the following set of protocols.
• Modbus Serial -RTU
• Modbus Serial - ASCII
• Modbus Ethernet
• OPC Client
2. In addition to the above protocols you can add new protocol drivers to this package. If
your device have other then Serial/Ethernet interface (Like USB, GPIB, CAN, ZibBee,
etc.,) you can also develop the protocol driver and add to this existing DA package.
To develop the protocol driver we will provide the driver development wizard.
3. This package will acquire the data continuously at the scan rate of 100 m.sec on
wards and data storage will be every one sec or above.
4. You can configure the number of I/O channels at filed level. At present you can
configure maximum of 200 channels of each type (i.e., AI, DI, AO, DO). With
software modification we can go beyond this limit.
5. Facility for Tag configuration for each channel.
6. Capable to configure and generate Alarms & Events on each channel.
7. Maximum of five (4 + 1) trend screens you can see at a time. In each trend screen you
can easily switch between Real time trend and Historic trend, vice versa.
8. In addition to the above trending we are providing Multi Channel Trend View. In this
view you can display maximum of 4 different channels (Either Historic or Real time
also both Analog and Discrete channels).
9. In Child Trend, capable to display multi channel mode. In this mode for X-Axis
instead of time you can use any Analog channel.
10. In addition to acquiring the data you can download the data (Holding Registers and
Output Coils) from this package to device.
11. Some more features such as Signal conditioning.
12. Password Protection & Login Management to add/remove DA package users.
13. Data Printing and Export to Excel files.
14. In addition to this we have an in-built Report View. With this report view you can
generate reports in three different modes for Historic data to submit daily reports.
15. Facility to add graphical screens (Like SCADA/HMI screens) on additional cost.
In addition to the above features, if you need any other customization (Such as graphic
screens, adding PID control blocks, new protocol driver development, etc.,) contact your
DA package vendor.
This package acquires the data only from one device (PLC, SLPC, etc., ) at a time. To get
the data from multiple devices, contact your package vendor for enhanced version of DA
package.
For the initial login to this package, type Administrator password as "AUTOMATION".
Once you entered into the package you can change the password.
