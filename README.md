# Platformer_Project_Game_Development
The platformer project made during the Game Development subject - Second year of videogame development

## Team
- Fernando Freixinet
- Marta Llurba

## Key features
- XML configuration file loading.
- Load and Save game state using XML file.
- Tiled TMX map loading and drawing with orthographic view.
- Map collision detection (platforms of the game).
- Map navigation: player movement and jumping.
- Title screen (press ENTER to continue).
- Player walk/jump/die/idle animations.
- Ending screen (on dying).
- DEBUG keys.
- Destroy enemies by shooting.
- Game items to recover health / lives.
- Game collectable items to accumulate points: coins, stars...
- Checkpoint autosave with feedback on passing.
- Audio feedback.
- Walking type enemy that can pathfind to the player.
- Flying enemy that can pathfind to the player avoiding non-walkable areas.
- Load and Save that considers each enemy state.
- Game capped to stable 60 frames per second.
- Game's movement normalized using dt (deltaTime).
- Entity System: All the elements in the game (Player, Enemies, Props, Coins / Collectibles) must hereby from a base Entity class and an EntityManager class must manage them (Initialize, include in a list, Update, Draw, CleanUp…)
- GUI: Title Screen Main Menu: It includes the following GuiButtons:
- - PLAY: Smooth transition to the gameplay screen.
- - CONTINUE: Only enabled if there is a saved game. It must transition to the last saved game. Use a different visible state for disabled!
- - SETTINGS: Open options menu. Including the following options:
- - - Adjust music volume, with a GuiSlider.
- - - Adjust fx volume, with a GuiSlider.
- - - Toggle fullscreen mode, with a GuiCheckBox.
- - - Toggle VSync, with a GuiCheckBox.
- - CREDITS: Open a credits panel where you can read the authors and the license.
- - EXIT: Quit the game.
- - NOTE: Buttons must be responsive and include:
- - - Visible state change on mouse-hover (FOCUSED)
- - - Visible state change on mouse-pressed (PRESSED)
- - - Generate an OnMouseClick event to be processed
- - - Audio feedback on mouse-hover and mouse-click.
- GUI: Gameplay Screen Pause Menu: Pressing ESCAPE triggers an in-game Pause Menu with the following options GuiButtons:
-  - RESUME: Continue playing the game after displaying the menu.
- - SETTINGS: Shows settings menu (same one from Title Screen).
- - BACK to TITLE: Returns to title screen. ○ EXIT: Quits the game.

## Controller:
- A: Move left
- D: Move right
- SPACE: Jump
- RIGHT CLICK: attack

## Debug Keys
- F5: Load the current game state
- F6: Load theprevious state
- F9: Show colliders
- F10: God Mode
