# Ford-Assembly-Plant-Sec-2-Group-1
![image](https://user-images.githubusercontent.com/85836332/205350463-b839fd4f-47f9-431f-8ee2-a79559cce178.png)
---------------------------------------------------------------------------------------------------------------------
Ford assembly line application

This is a simulated HMI for a Ford Assembly Line.
This simulates the building of the body, paint, chassis, and interior of F150 and Expedition models
The HMI will monitor the inventory levels of all machines along the assembly line, monitor the status,
temperature, and humidity of paint machines. This HMI visually represents the vehicles being created 
and allows users to update/regulate all machines in the plant.

---------------------------------------------------------------------------------------------------------------------
Assembly Line Stages

Order - View the requested vehicle to be made

BodyMachine - View/monitor/manage the panels used to build the vehicle

![image](https://user-images.githubusercontent.com/85836332/205351547-4ce625cd-f393-4a14-9178-1565a18ec91e.png)

PaintMachine - View/monitor/manage the painting of the vehicle panels

![image](https://user-images.githubusercontent.com/85836332/205351595-3dc036c9-191f-4f99-9287-f5a1c5f0eefb.png)

ChassisMachine - View/monitor/manage the addition of the chassis/engine

![image](https://user-images.githubusercontent.com/85836332/205351655-1ab194a6-7315-45af-9c2b-763d40e52028.png)

InteriorMachine - View/monitor/manage the addition of the interior to the vehicle

![image](https://user-images.githubusercontent.com/85836332/205351734-2b160d1f-224f-439e-9a19-6c8d3af93b17.png)

---------------------------------------------------------------------------------------------------------------------
HMI Functionality  

Switch bays/lines

![image](https://user-images.githubusercontent.com/85836332/205352467-ee9c50c8-2517-41f0-a0b3-327c767921ee.png)

Restock bays/lines - a bay/lines cannot be restocked while it is in use

![image](https://user-images.githubusercontent.com/85836332/205352543-6f0462dc-0caf-493b-9e3f-141d66f1638a.png)

Read logs from all machines

![image](https://user-images.githubusercontent.com/85836332/205352915-11496933-7427-4416-87c5-87bac97d6338.png)

Regulate paint machine temperature, humidity, paint/fluid levels

![image](https://user-images.githubusercontent.com/85836332/205352645-2cd4d164-949a-4e09-93f4-6f80566eae65.png)

View the progression of a vehicle being created - Completed Vehicles are assigned a VIN and are saved

![image](https://user-images.githubusercontent.com/85836332/205352824-9e453b13-6193-4d75-8a29-04bb90cf7e7a.png)

Warnings - the assembly line will be paused

![image](https://user-images.githubusercontent.com/85836332/205351341-8f274f47-afa3-4dc2-86c2-3eae4ced2a35.png)

![image](https://user-images.githubusercontent.com/85836332/205356666-5c3bbe27-d4e2-4151-b0b1-da165a32ce63.png)

Critical Warnings - the assembly will be paused

![image](https://user-images.githubusercontent.com/85836332/205357090-888e9436-99f7-4ca4-8466-7d2c7172314b.png)

---------------------------------------------------------------------------------------------------------------------
TypeQuest Creators 

Brodin Collins-Robb |
Owen Garland |
Tyler Scheifley |
Zebadiah Lake

Interface Library Used:

This project uses the Graphical User Interface ImGui and relies opengl and glfw as a backend for the interface
https://github.com/ocornut/imgui

---------------------------------------------------------------------------------------------------------------------
