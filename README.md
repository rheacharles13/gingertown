# GingerTown 
## Final Project, Creative Embedded Systems (Fall 2024)
<p align="center">
  <img src="./gingerhouse.gif" alt="GingerHouse GIF">
</p>

### Team: Hari Bhimaraju (mhb2189), Rhea Charles (rtc2133)

## Project Goal
In our final project, we will build an interactive gingerbread house "village" using two ESP32s, LED lights, buttons, and motion sensors. The project will emulate two neighbors "speaking" to each other over WiFi by transmitting a simple greeting message when a neighbor presses the doorbell (which the neighbor returns) and turning on the porch light when they wave. 

## Materials
- Full-size breadboard
- [ESP32 TTGO T-displays](https://lilygo.cc/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board?srsltid=AfmBOopwznDFZNyBDXsGDDsJCy1XqfVx2fuSF5gfMCL8IUSiKmo8Y2OH)
- USB-C cable with working data connections
- [Motion Sensors](https://www.digikey.com/en/products/detail/murata-electronics/IRA-S210ST01/5012561)
- Buttons
- LEDs
- Wires of varying lengths
- 1-ply chipboard

## Connection Instructions
Connect your ESP-32 to a computer using the USB-C cable. Open the included [gingertown.ino](./gingertown.ino), [still.h](./still.h), and [wave.h](./wave.h) files in your [Arduino IDE](https://www.arduino.cc/en/software). Compile and upload the main file to your ESP-32 using the buttons on the top left corner of the IDE. 

#### **Note**: Make sure to change the otherDeviceMAC[] value in line 19 of [gingertown.ino](./gingertown.ino) to the hexadecimal representation of the other MAC address you are communicating with. More information on how to find this address is commented in [gingertown.ino](./gingertown.ino).

### Laser-Cutting Enclosure
Upload the included [gingerhouse_plan.svg]((./gingerhouse_plan.svg)) file (which is originally from [3axis.co](https://3axis.co/laser-cut-simple-house-template-svg-file/m1d6vqe7/#google_vignette)) to Adobe Illustrator. Resize and adjust the file to fit your breadboard dimensions. For our full-size breadboard, we resized the elements so that the front piece of the house would stand 10 inches tall from the ground to the top of the roof. In your laser cutting software, set all edges to 'cut' except for the windows, which should be 'engraved' but not cut. Place your piece of 1-ply chipboard in the laser cutter and calibrate as needed – press start on your laser cutter.

#### **Note**: Do not look directly at the laser while it is on. However, keep an eye on the material to ensure no fires are starting. Stop your design if a fire does start while cutting.

## Assembly Instructions
Easily fit the notches of the laser cut pieces together to construct your gingerbread house, leaving out the front of the house. Trace the breadboard and outline of mounted components (door (ESP-32), porch light (LED), doorbell (button), and motion sensor) on the front of the house. Next, use an X-ACTO knife to cut out the appropriate holes for the components and fit the breadboard with components on the back of the piece. Secure with packing tape before fitting the front piece to the rest of the house.

## Detailed Documentation
For more detailed documentation on our design process and the technical challenges we faced, please visit our [Notion page](https://www.notion.so/Final-Project-GingerTown-1580c243667880daa0d8d64ad89ab157).
