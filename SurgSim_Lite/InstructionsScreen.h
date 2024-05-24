#pragma once

// avatar
//
//  _     ____  ____  ____  _  _        _     _ _____ _____
// / \   /  _ \/  __\/ ___\/ \/ \__/|  / \   / Y__ __Y  __/
// | |   | / \||  \/||    \| || |\/||  | |   | | / \ |  \
// | |_/\| |-|||  __/\___ || || |  ||  | |_/\| | | | |  /_
// \____/\_/ \|\_/   \____/\_/\_/  \|  \____/\_/ \_/ \____\

template<int NR, int NC>
void draw_instructions(SpriteHandler<NR, NC>& sh, int max_health)
{
  sh.write_buffer("_     ____  ____  ____  _  _        _     _ _____ _____", 1, 11, Text::Color::White);

  sh.write_buffer("/ \\   /  _ \\/  __\\/ ___\\/ \\/ \\__/|  / \\   / Y__ __Y  __/", 2, 10, Text::Color::White);

  sh.write_buffer("| |   | / \\||  \\/||    \\| || |\\/||  | |   | | / \\ |  \\", 3, 10, Text::Color::White);

  sh.write_buffer("| |_/\\| |-|||  __/\\___ || || |  ||  | |_/\\| | | | |  /_", 4, 10, Text::Color::White);

  sh.write_buffer("\\____/\\_/ \\|\\_/   \\____/\\_/\\_/  \\|  \\____/\\_/ \\_/ \\____\\", 5, 10, Text::Color::White);

  sh.write_buffer("The objective of this game is to complete the operation by ", 7, 2, Text::Color::White);
  sh.write_buffer(" properly sealing the cystic artery and the cystic duct and ", 8, 2, Text::Color::White);
  sh.write_buffer(" then dividing them. The bladder resection part is not done here.", 9, 2, Text::Color::White);
  sh.write_buffer("It is game over when you get too much bleeding, ", 10, 2, Text::Color::White);
  sh.write_buffer(" the health of the patient has decreased to zero or if you have damaged ", 11, 2, Text::Color::White);
  sh.write_buffer(" important structures such as ductus choledochus.", 12, 2, Text::Color::White);
  //sh.write_buffer(" such as ductus choledochus.", 13, 2, Text::Color::White);
  //sh.write_buffer(" They will give you 5 additional health units. Max health is " + std::to_string(max_health) + ".", 14, 2, Text::Color::White);
  //sh.write_buffer("You can see the UFOs in the radar which appear as dots around the frame:", 15, 2, Text::Color::White);
  //sh.write_buffer(" black    .       : UFO is far away and patrolling.", 16, 2, Text::Color::White);
  //sh.write_buffer(" black    o       : UFO is nearby and patrolling.", 17, 2, Text::Color::White);
  //sh.write_buffer(" blinking . and o : UFO is hunting you!", 18, 2, Text::Color::White);
  //sh.write_buffer(" dark red o       : UFO is trying to shoot at you!", 19, 2, Text::Color::White);
  //sh.write_buffer(" cyan     o       : UFO evades.", 20, 2, Text::Color::White);

  int controls_r0 = 15;
  sh.write_buffer("Controls:", controls_r0+0, 2, Text::Color::White);
  sh.write_buffer(" a,d : controls the angle of the left instrument.", controls_r0+1, 2, Text::Color::White);
  sh.write_buffer(" s,w : controls the shaft insertion of the left instrument.", controls_r0+2, 2, Text::Color::White);
  sh.write_buffer(" <,>-arrows : controls the angle of the right instrument.", controls_r0+3, 2, Text::Color::White);
  sh.write_buffer(" v,^-arrows : controls the shaft insertion of the right instrument.", controls_r0+4, 2, Text::Color::White);
  sh.write_buffer(" f,j : controls grasping, cutting, clipping and suction on either side.", controls_r0+5, 2, Text::Color::White);
  sh.write_buffer(" g,h : controls rinsing on either side using the suction/rinsing device.", controls_r0+6, 2, Text::Color::White);
  sh.write_buffer(" , : thermal cut pedal for either hook tool.", controls_r0+7, 2, Text::Color::White);
  sh.write_buffer(" . : thermal coag pedal for either hook tool.", controls_r0+8, 2, Text::Color::White);
  sh.write_buffer(" _ : shows or hides the tool menus.", controls_r0+9, 2, Text::Color::White);
  //sh.write_buffer(" space-bar : fire", 24, 2, Text::Color::White);
  //sh.write_buffer(" q : quit", 25, 2, Text::Color::White);

  sh.write_buffer("Be aware! The controls are a bit wonky so be careful of ", 26, 2, Text::Color::White);
  sh.write_buffer(" pressing a key too long!", 27, 2, Text::Color::White);

  sh.write_buffer("Press space-bar to begin...", 29, 25, Text::Color::White);
}
