
# <span style="color: yellow;">Author: <span style="color:red;"> Group-3<br>
Date: 10/03/2023 <br>
# <span style="color: green;">MainInterface Class</span>


## <span style="color: aqua;">Description

The `MainInterface` class is responsible for creating a window and displaying a default maze grid. It utilizes the SFML library to handle the graphical representation of the maze and its entities.
<details> <summary> Functionalities </summary>

### <span style="color: orange;">MainInterface Constructor

The `MainInterface` constructor initializes an instance of the `MainInterface` class, creating a window and menu.
- Setting up the SFML window with the specified dimensions and title
- Initializing the menu for any additional user interface elements

###  <span style="color: orange;">DrawGrid Function

The `DrawGrid` function is a part of the `MainInterface` class and is responsible for drawing the maze grid and entities on the SFML window. It utilizes the SFML library to display the maze grid, walls, agents, and other elements on the graphical interface. This function takes care of rendering the maze grid based on the provided parameters and updating the window accordingly.

The `DrawGrid` function consists of the following main functionalities:
- Clearing the SFML window
- Creating a vector representation of the maze grid
- Calculating the cell size for each element in the grid
- Drawing the walls, empty cells, and agent cells with their respective textures and colors
- Displaying the menu if required
- Updating the window display with the latest grid and entity information

The function ensures that the maze grid is properly displayed with the appropriate symbols and textures, making it easily understandable and visually appealing to the user.

Please note that the `DrawGrid` function is a crucial part of the graphical representation of the maze and should be appropriately integrated into the overall functionality of the `MainInterface` class.


###  <span style="color: orange;">SelectAction Function

The `selectAction` function is a part of the `MainInterface` class and is responsible for handling user input and selecting actions based on the user's interactions with the maze grid and entities. It utilizes the SFML library to capture and process user input events, enabling the user to interact with the maze application effectively.

The `selectAction` function performs the following key tasks:
- Continuously checks for user input events and actions within the SFML window
- Processes various types of events, including key presses, mouse movements, and window resizing
- Determines the appropriate action based on the specific key or event triggered by the user
- Updates the graphical representation of the maze grid accordingly

The function ensures that the user's interactions with the maze application are accurately captured and processed to facilitate smooth and intuitive gameplay or navigation within the maze environment.

</details>

# <span style="color: green;">Button Class</span>


## <span style="color: aqua;">Description

<details> <summary>Functionalities:</summary>

### <span style="color: orange;"> Button Constructor
</details>