# 🏎️Team F1 – WRO Future Engineers 2025🏁
</p>
</p>
<br>
<br>
<br>
<p align="center">
  <img width="400" src="https://github.com/user-attachments/assets/dcaa8bc4-9314-4e37-97fa-51abc5f054b7"/>
  <img  width="450" src= "https://github.com/user-attachments/assets/1291b326-65d5-40d3-8a7d-3f70c56097e7"/> 
</p>
</p>
<br>

---
## Table of Contents
- [About Team 👥](#about-team-)
- [Project Overview 🧠](#project-overview-)
- [Timeline 🗓️](#timeline-)
- [Robot Design](#robot-design)


###  About Team 👥

Picture

**Introducing Team F1 from Yothinburana School**

We are a group of students from **Yothinburana School** who share a strong passion for programming and robotics. All three of us are active members of the school’s **Robotics Club**, which has around 30 members. We each joined the club in **Grade 7 (Mathayom 1)**, driven by a love for technology and building things from scratch.

Our team is called **"F1"**, inspired by our shared enthusiasm for **Formula 1 racing**. Beyond just being fans, we dream of one day designing and building our own **self-driving vehicle** — a goal that keeps us motivated and united as a team.

<br>

### 🧑‍🤝‍🧑 Meet the Team

| Name       | Role                           | Responsibility                                                                 |
|------------|--------------------------------|--------------------------------------------------------------------------------|
| **Chaiwat Chinsupawat**  | 📚 Document Designer               | Handles documentation and presentations        |
| **Pongpapat Putongkam**          | 🛠️ Mechanic                    | Designs and assembles the robot’s hardware and structural components          |
| **Peradon Nimsongprasert**            | 💻 Programmer                  | Codes and refines the robot’s software and logic                              |

> 🧒 Peradon is two years younger than Chaiwat and Pongpapat but brings equal energy and skill to the team!

<br>

### 🤝 How We Work

Even though we come from different age groups, we collaborate seamlessly. Each of us brings **unique strengths** to the team, and our clearly divided roles allow us to stay focused and productive.

We believe in:
- 🔄 Continuous collaboration
- 📖 Learning from one another
- 💡 Innovating with each challenge

<br>

### 💬 Our Mission

> _"We’re not just building robots — we’re building a future."_  

Our goal is to **innovate, build**, and one day **revolutionize autonomous vehicles**. Every challenge we face is a chance to grow, both as engineers and as a team.

<br>

in another and grow as future engineers. No matter the challenge, we stay focused on our shared dream:  
**to innovate, build, and someday revolutionize how autonomous vehicles are made.**

---


## Project Overview 🧠

Our goal is clear: **Design, build, and innovate our own self-driving robot.**

We believe in:
- 🚀 Innovation through experimentation  
- 💬 Learning through teamwork  
- 🧩 Problem-solving with creativity  
- 🛠️ Applying tech to real-world challenges  


---

## Timeline 🗓️

| Phase               | Month     | Goal                                           |
|---------------------|-----------|------------------------------------------------|
| 🔍 Research         | July      | Understand LIDAR, OpenMV, and line tracking    |
| 🛠️ Prototyping      | July      | Assemble base robot + test sensor input        |
| 🧠 Programming      | August    | Implement PID control and obstacle logic       |
| 🧪 Testing          | August    | Real-world simulation and debugging            |
| 🏁 Final Submission | September | Polish and submit for WRO competition          |

---

## Robot Design

For the past 2 years, we have been competing in the **WRO Future Engineer** and have advanced to the international round twice:  
- 🌎 **WRO PANAMA 2023**  
- 🌎 **WRO TÜRKİYE 2024**  

Our current robot design builds on the strengths of our previous generation robots, focusing on **precision, durability, and performance**.

---

### 🖥️ Design Software

This year, we started with 5 different design tools.  
Although **Blender** is very easy and straightforward to use, we decided to switch to a professional tool for the first time — **Autodesk Fusion 360**.  
With improvements in our electronic components, our mechanical hardware needed to be more precise.  
Fusion 360 provides **millimeter-level precision**, which is essential for our designs.

---

### ⚙️ Design Hardware

#### Bambu Lab X1C 3D Printer

<p align="center">
  <img width="500" height="500" alt="Design Hardware" src="https://github.com/user-attachments/assets/fc799c5c-7fab-481d-bad7-ec25c413d730" />
</p>

We chose the Bambu Lab X1C 3D Printer because it is widely recognized and popular among students and beginners due to its ease of use. Unlike many professional 3D printers, it is not complex, making it ideal for learning and rapid prototyping. Despite its simplicity, the print quality is professional-grade, delivering highly detailed and precise results. Additionally, the printer is affordable relative to the performance it provides, offering excellent value for money compared to other printers on the market. This combination of usability, quality, and cost-effectiveness makes it a perfect choice for both educational and practical applications.

**🖨️ Product Features of Bambu Lab X1C**

- 🎨 **Multi-Color & Multi-Material Capability** – Print with different colors and materials in a single job.  
- ✅ **Proven Reliability** – Trusted by professionals worldwide.  
- 📏 **High-Quality Printing** – Achieves 7 μm Lidar resolution for exceptional detail.  
- ⚡ **High-Speed CoreXY** – Up to 20,000 mm/s² acceleration for rapid prints.  
- 🛠️ **Dual Auto Bed Leveling** – Ensures perfect first layers every time.  
- 🔄 **AMS 2 Pro Compatibility** – Works seamlessly with X1/P1 series printers for multi-material printing.

link:https://asia.store.bambulab.com/products/x1-carbon?from=navigation

---

### Steering Mechanic Part
<p align="center">
<img width="732" height="352" alt="image" src="https://github.com/user-attachments/assets/674a9b35-0594-456e-b30d-0957e6978937" />


This Future Engineering mission, teams need to build a self-driving car and the steering mechanic determind its direction.
In our previous generation robot, we use the "Parallel steering" mechanism in which the wheels angle is equal on both side. With this
mechanism, we encounter alot of sliping at both front and back of the car which made it drift and lose its balance. So after our research,
we decided to use the "Akerman steering" mechanism. But there are two type of this particular mechanism; "Akerman",which is use globally in any manufactured cars and "Anti-Akerman", which is commonly use in formula series racing cars. "Akerman steering" is when a car is turning, the steering angle is different on both wheel with a tigher angle on the inside and a wider angle on the other side because while turnng the inside wheel turn at a smaller radius while the outside wheel is turning at a larger radius. This eliminate the scrubing(sliding) of the tires and made the turn more smooth. The "Anti-Akerman" on the other hand is the opposite with the outside wheel turnig at a thigher angle generate more grip which is need in a formula car, but it is more difficult to control (the down force generate by formula cars compensate these flaws).
In short, the "Akerman steering" mechanism is out best choice and will definitely improve out robot.

But how do we actually steer the car? The answer is servo motor, specificly the "Geek servo 360". It is a very compact yet powerful servo motor with a large turning radius of 360 degree and maximum torque of 1.6kg/cm. Turning speed of 60°/0.14s and only weight 20g. This servo include a Lego cmpadible connection point which make it very simple to attach to the main frame of the robot.  

#### Geek servo 360

<p align="center">
<img width="300" height="300" alt="image" src="https://github.com/user-attachments/assets/f55ca626-15f2-439b-b3c0-39458cbd5383"/>

  
|Power Usage     |       |
|------------|---------|
|Operating Voltage | 3.3V~|
|Rated Voltage | 4.8V|
|Rated Current | 70mA|
|Blocked Rotor Current | 900mA|
|Slipping Current | 700mA|

Link:https://th.aliexpress.com/item/1005005515178664.html?gatewayAdapt=glo2tha
**Drive and Power Train**
<br>
