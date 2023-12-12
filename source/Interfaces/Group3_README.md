
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

The `Button` class is responsible for drawing rectangles on the RenderWindow and handling mouse's hover and click events

<details> <summary>Functionalities:</summary>


### <span style="color: orange;"> Button Constructor

Construct the text with the font in the asset folder. Set the text to be displayed by the button and set the properties of the text and rectangleshape by the parameters passed to the constructor.

### <span style="color: orange;"> Setters
SetString : 
SetFont : 
SetButtonSize : 
SetBackGroundColor : 
SetTextColor : 
SetPosition

### <span style="color: orange;"> DrawTo

Draws itself onto the RenderWindow

### <span style="color: orange;"> IsMouseOver

Check mouse position and return true if it's on the button

</details>










# <span style="color: green;">TextureHolder Class</span>


## <span style="color: aqua;">Description

The `TextureHolder` class is responsible for creating and storing references to sf::Texture objects. It is given a file, generates an sf::Texture, and readily gives a reference to the generated texture when asked.
<details> <summary> Functionalities </summary>

###  <span style="color: orange;">LoadTexture Function

The `LoadTexture` function is a part of the `TextureHolder` class and is responsible for generating sf::Textures from a given file path. 

The `LoadTexture` function consists of the following main functionalities:
- Attempt to generate sf::Texture using given file path
- Store the generated sf::Texture in a map using the id as a key

The function allows new sf::Textures to be instantiated and stored to the class for future reference calling by id.


###  <span style="color: orange;">GetTexture Function

The `GetTexture` function is a part of the `TextureHolder` class and is responsible for getting a reference to the initialized sf::Texture with the matching id. 

The `GetTexture` function performs the following key tasks:
- Determine if there is a loaded texture with the given id
- Return the dereferenced sf::Texture that matches the id

The function allows for easy reference to any of the loaded sf::Textures without having to instantiate new ones for each use. 

</details>

# <span style="color: green;">MessageBoard Class</span>

## <span style="color: aqua;">Description

<details> <summary>Functionalities:</summary>

### <span style="color: orange;"> MessageBoard Constructor

The `MessageBoard` class is responsible for managing and displaying messages on the interface entity.
Parameters: - **font:** The font used by the message board.

### <span style="color: orange;"> DrawTo Method

Draws the message board onto the render window. If the elapsed time since the last message exceeds 10 seconds, the message board will be cleared.
Parameters:- **window:** The render window to be drawn on.

</details>

# <span style="color: green;">TextBox Class</span>

## <span style="color: aqua;">Description

<details> <summary>Functionalities:</summary>

### <span style="color: orange;"> TextBox Constructor

The `TextBox` class enables the interface entity to send messages to the `MessageBoard` and `World`. It provides functionality for handling text input, setting the string, and drawing the text to the render window.

#### Parameters:
- **font:** The font used by TextBox.
- **size:** The size of the text.
- **color:** The color of the text.
- **sel:** Set the activity of TextBox.

### <span style="color: orange;"> SetString Method

Sets the string of the text in the TextBox.
Parameters:- **s:** The string to be set.

### <span style="color: orange;"> SetSelected Method

Setter for isSelected, updates the text displayed in the box based on selection status.
Parameters:- **sel:** Whether the TextBox is to be set selected or not.

### <span style="color: orange;"> TypedOn Method

TypedOn event handler, processes new text that has been typed into the TextBox.
Parameters:- **input:** The new text event.

### <span style="color: orange;"> InputLogic Method

Determines what actions to take with different key presses.
Parameters:- **charTyped:** The character that has just been entered into the TextBox.

### <span style="color: orange;"> DeleteLastChar Method

Removes the last character from the TextBox's current string.

### <span style="color: orange;"> DrawTo Method

Draws the text and border to the render window.
Parameters:- **window:** The render window to be drawn on.

</details>


