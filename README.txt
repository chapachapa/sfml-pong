GAME 230 Assignment 4 - PONG
by Charles Park

Please open "/x64/Release/Assignment 4 Pong.exe" to play the game.

Press Enter on the main screen
Press 1 to play against AI
Press 2 to play against another player.

Player 1 controls:
- W: Up
- S: Down

Player 2 controls:
- Up Arrow: Up
- Down Arrow: Down

Other controls:
- P: Pauses the game

Known issues:
- The ball can sometimes randomly escape the arena. I've placed a clamp function in hopes of keeping the ball within the arena, but this may have unintended consequences that I didn't have time to test it out yet.

Other notes on the AI
- There are two factors to the AI's movement
1. when it is not tracking down the predicted position it'll move to a random position, wait a few, then move again to another random position.
2. there's a short random delay before the AI changes its state to tracking (max random of this delay gets slightly shortened on every paddle hit)
3. once it starts to track, it moves to the target position.
4. once hit, the paddle goes back to idle movement.