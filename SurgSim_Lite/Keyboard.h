#pragma once
#include <Termin8or/input/Keyboard.h>

Key register_keypresses(const t8::KeyPressDataPair& kpdp)
{
  Key curr_special_key = Key::None;
  auto key = get_char_key(kpdp.transient);
  auto special_key = get_special_key(kpdp.transient);

  if (special_key == t8::SpecialKey::Left)
    curr_special_key = Key::RI_Left;
  else if (special_key == t8::SpecialKey::Right)
    curr_special_key = Key::RI_Right;
  else if (special_key == t8::SpecialKey::Down)
    curr_special_key = Key::RI_Down;
  else if (special_key == t8::SpecialKey::Up)
    curr_special_key = Key::RI_Up;
  else if (str::to_lower(key) == 'a')
    curr_special_key = Key::LI_Left;
  else if (str::to_lower(key) == 'd')
    curr_special_key = Key::LI_Right;
  else if (str::to_lower(key) == 's')
    curr_special_key = Key::LI_Down;
  else if (str::to_lower(key) == 'w')
    curr_special_key = Key::LI_Up;
  else if (key == ' ')
    curr_special_key = Key::Skip;
  else if (key == ',')
    curr_special_key = Key::Cut;
  else if (key == '-')
    curr_special_key = Key::Menu;
  else if (key == '.')
    curr_special_key = Key::Coag;
  else if (str::to_lower(key) == 'f')
    curr_special_key = Key::LI_Trigger;
  else if (str::to_lower(key) == 'j')
    curr_special_key = Key::RI_Trigger;
  else if (str::to_lower(key) == 'g')
    curr_special_key = Key::LI_Trigger2;
  else if (str::to_lower(key) == 'h')
    curr_special_key = Key::RI_Trigger2;

  return curr_special_key;
}

