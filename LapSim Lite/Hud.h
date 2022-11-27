#pragma once

void draw_hud(SpriteHandler& sh,
              int health, int max_health,
              int blood, int max_blood,
              int score)
{
  int hud_c = 1;
  if (health < 0)
    health = 0;
  std::string health_char = "h";
  switch (health % 3)
  {
    case 2: health_char = "*"; break;
    case 1: health_char = "o"; break;
    case 0: health_char = ""; break;
  }
  std::string life_str = "Health: ["
    + str::rep_char('#', std::floor(health/3.f))
    + health_char
    + str::rep_char(' ', std::ceil(max_health/3.f) - std::ceil(health/3.f))
    + "]";
  sh.write_buffer(life_str, 0, hud_c, Text::Color::Cyan);
  //sh.write_buffer(std::to_string(health), 5, 5, Text::Color::Black);
  hud_c += 1 + life_str.size();
  if (blood > max_blood)
    blood = max_blood;
  std::string blood_char = "b";
  switch (blood % 3)
  {
    case 2: blood_char = "*"; break;
    case 1: blood_char = "o"; break;
    case 0: blood_char = ""; break;
  }
  std::string blood_str = "Blood: ["
    + str::rep_char('#', std::floor(blood/3.f))
    + blood_char
    + str::rep_char(' ', std::ceil(max_blood/3.f) - std::ceil(blood/3.f))
    + "]";
  sh.write_buffer(blood_str, 0, hud_c, Text::Color::Cyan);
  hud_c += 1 + blood_str.size();
  std::string score_str = "Score: ";
  score_str += std::to_string(score);
  sh.write_buffer(score_str, 0, hud_c, Text::Color::Cyan);
}
