#pragma once
#include <Termin8or/Keyboard.h>

Key register_keypresses(const keyboard::KeyPressData& kpd)
{
  Key curr_special_key = Key::None;
  auto key = keyboard::get_char_key(kpd);
  auto special_key = keyboard::get_special_key(kpd);

  if (special_key == keyboard::SpecialKey::Left)
    curr_special_key = Key::RI_Left;
  else if (special_key == keyboard::SpecialKey::Right)
    curr_special_key = Key::RI_Right;
  else if (special_key == keyboard::SpecialKey::Down)
    curr_special_key = Key::RI_Down;
  else if (special_key == keyboard::SpecialKey::Up)
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

