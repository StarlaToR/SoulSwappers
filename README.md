<h1> Soul Swappers </h1>
<img src = "Images_README/MainMenu.png">
<h2><b>Introduction </b></h2>
 Soul Swappers is a cooperative runner shooter on Switch. You incarnate two wizard's souls who share a same body to reach the center of your destroyed tower. To acheive your goals, you should run through the debris of your old home and fight your old tools which came alive magically.

Soul Swappers is a project realised on Unreal Engine 4.27 mainly in C++. The project is designed for Nintendo Switch and PC. The people who took part of the project are Lenny Rabinne, Mateo Erbisti, Antoine Mordant and Bryan Bachelet in Isart Digital.

<h2><b> Controls :</b></h2>
<ul>
<li> For the wizard :
    <ul> 
    <li> Press A to jump
    <li> Hold A to wall run
    <li> Use left joystick to move right or left
    <li> Start Button to set pause
    </ul>
<li> For the spirit :
    <ul>
        <li> Right Trigger to shoot
        <li> Left shoulder button to swap bodies
        <li> Start Button to set pause
    </ul>
</ul>


<h2><b> Gameplay :</b> </h2>
<img src = "Images_README/ScreenPLay1.png">
In a run, you control one of the two avatars : the runner and the shooter, respectively represented by the wizard and the spirit. Each player is assigned to a color. This color allows the magician to walk on the platforms with the same color and the spirit to shoot the enemies with the same color. You will have to change roles with your game partner between shooter and runner according to your environnement. To reach the center of your blown up home, you will have to communicate with your partner to be able to progress through the level.
<img src = "Images_README/BluePlayerScreen.png">

The shooter has different weapons : 
<ul>
<li> Magical Beam
<li> Explosive Fireball
<li> Fire Rain
</ul>

<img src = "Images_README/laserBlue.png">
<img src = "Images_README/SwapScreen.png">

<h2><b>Personnal investement</b></h2>

<h3>Enemies</h3>

<p>I have created the enemy behavior and their management. Each enemy follow the player until it is close enough and then go in straight line to allow the player to dodge them. That is the easy part.
Our game is designed for Nintendo Switch so instancing enemies in runtime would require too much resources. To solve this problem, i implemented an enemy pool. This is a stock of pre-instanciated enemies controlled by a manager that will move them where they are needed.</p>

<h3>Level Design</h3>

<p>We needed the ennemies to spawn in different configurations but we would not have the time to place it by  hand. So i programmed enemy spawner with different pre-made swpan patterns such as straight line, circle etc.</p>

<h3>User Interface</h3>

<p>Lastly, i have managed the different menus and UI of the game.</p>
