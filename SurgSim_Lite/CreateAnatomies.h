#pragma once
#include "AnatomyLineData.h"

const float coag_sufficient = 0.8f;

//   00000000001111111111222222222233333333334444444444555555555566666666667777777777
//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
//00 +------------------------------------------------------------------------------+
//01 |                                                                              |
//02 |                                                                              |
//03 |                                                                              |
//04 |                                      ###############                         |
//05 |                                  ##########################                  |
//06 |                              #####################################           |
//07 |                           ##############################################     |
//08 |                        ######################################################|
//09 |                      ########################################################|
//10 |                    ##########################################################|
//11 |                  ############################################################|
//12 |                ##############################################################|
//13 |              ################################################################|
//14 |             #################################################################|
//15 |            ##################################################################|
//16 |           ###################################################################|
//17 |          ####################################################################|
//18 |         #####################################################################|
//19 |        ######################################################################|
//20 |       #######################################################################|
//21 |      ########################################################################|
//22 |    ########################################################################  |
//23 |              #######################################################         |
//24 |                                                                              |
//25 |                                                                              |
//26 |                                                                              |
//27 |                                                                              |
//28 |                                                                              |
//29 +------------------------------------------------------------------------------+

void create_liver(std::vector<AnatomyLineData>& liver_texture)
{
  auto fg_color = Color::Red;
  auto fg_cut_color = Color::DarkGray;
  Gradient<Color> bg_color_gradient
  {
    {
      { 0.f, Color::Red },
      { 0.4f, Color::Magenta },
      { coag_sufficient, Color::White },
      { 0.9f, Color::LightGray },
      { 0.95f, Color::DarkGray },
      { 1.f, Color::Black }
    }
  };

  create_texture_row_uniform(liver_texture, '#', '#', '#', 4, 39, 53, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 5, 35, 60, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 6, 31, 67, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 7, 28, 73, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 8, 25, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 9, 23, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 10, 21, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 11, 19, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 12, 17, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 13, 15, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 14, 14, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 15, 13, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 16, 12, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 17, 11, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 18, 10, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 19, 9, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 20, 8, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 21, 7, 78, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 22, 5, 76, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(liver_texture, '#', '#', '#', 23, 15, 69, fg_color, fg_cut_color, bg_color_gradient);
}


//   00000000001111111111222222222233333333334444444444555555555566666666667777777777
//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
//00 +------------------------------------------------------------------------------+
//01 |                                                                              |
//02 |                                                                              |
//03 |                                                                              |
//04 |                                      ###############                         |
//05 |                                  ##########################                  |
//06 |                              ###########ooooooooo#################           |
//07 |                           #############oOOOOOOOOOo######################     |
//08 |                        ###############oOOOOOOOOOOo###########################|
//09 |                      #################oOOOOOOOOOOo###########################|
//10 |                    ###################oOOOOOOOOOo############################|
//11 |                  #####################oOOOOOOOOOo############################|
//12 |                ########################oOOOOOOOo#############################|
//13 |              ##########################oOOOOOOOo#############################|
//14 |             ###########################oOOOOOOo##############################|
//15 |            ############################oOOOOOOo##############################|
//16 |           #############################oOOOOOo###############################|
//17 |          ##############################oOOOOoo###############################|
//18 |         ################################ooooo################################|
//19 |        ######################################################################|
//20 |       #######################################################################|
//21 |      ########################################################################|
//22 |    ########################################################################  |
//23 |              #######################################################         |
//24 |                                                                              |
//25 |                                                                              |
//26 |                                                                              |
//27 |                                                                              |
//28 |                                                                              |
//29 +------------------------------------------------------------------------------+

void create_gallbladder(std::vector<AnatomyLineData>& gallbladder_texture)
{
  auto fg_color = Color::DarkCyan;
  auto fg_cut_color = Color::DarkGray;
  Gradient<Color> bg_color_gradient
  {
    {
      { 0.f, Color::LightGray },
      { coag_sufficient, Color::White },
      { 0.9f, Color::DarkGray },
      { 1.f, Color::Black }
    }
  };
  
  create_texture_row_uniform(gallbladder_texture, 'o', 'o', 'o', 6, 42, 50, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 7, 41, 51, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 8, 40, 51, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 9, 40, 51, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 10, 40, 50, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 11, 40, 50, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 12, 41, 49, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 13, 41, 49, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 14, 41, 48, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 15, 41, 48, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 16, 41, 47, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'O', 'o', 17, 41, 47, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(gallbladder_texture, 'o', 'o', 'o', 18, 42, 46, fg_color, fg_cut_color, bg_color_gradient);
}

//   00000000001111111111222222222233333333334444444444555555555566666666667777777777
//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
//00 +------------------------------------------------------------------------------+
//01 |                                                                              |
//02 |                                                                              |
//03 |                                                                              |
//04 |                                      ###############                         |
//05 |                                  ##########################                  |
//06 |                              ###########ooooooooo#################           |
//07 |                           #############oOOOOOOOOOo######################     |
//08 |                        ###############oOOOOOOOOOOo###########################|
//09 |                      #################oOOOOOOOOOOo###########################|
//10 |                    ###################oOOOOOOOOOo############################|
//11 |                  #####################oOOOOOOOOOo############################|
//12 |                ########################oOOOOOOOo#############################|
//13 |              ##########################oOOOOOOOo#############################|
//14 |             ###########################oOOOOOOo##############################|
//15 |            ############################oOOOOOOo##############################|
//16 |           #############################oOOOOOo###############################|
//17 |          ##############################oOOOOoo###############################|
//18 |         ################################ooooo################################|
//19 |        ##################################|#|#################################|
//20 |       ###################################|#|#################################|
//21 |      ##################################--|-+#################################|
//22 |    ###################################---+#|###############################  |
//23 |              ############################|#|########################         |
//24 |                                          | |                                 |
//25 |                                                                              |
//26 |                                                                              |
//27 |                                                                              |
//28 |                                                                              |
//29 +------------------------------------------------------------------------------+

void create_artery_tree(std::vector<AnatomyLineData>& artery_tree_texture)
{
  auto fg_color = Color::Magenta;//Color::Magenta;
  //auto bg_color = Color::DarkMagenta;
  Gradient<Color> bg_color_gradient
  {
    {
      { 0.f, Color::DarkMagenta },
      { 0.4f, Color::Magenta },
      { coag_sufficient, Color::White },
      { 0.9f, Color::LightGray },
      { 0.95f, Color::DarkGray },
      { 1.f, Color::Black }
    }
  };
  
  create_texture_row_tube(artery_tree_texture, "|", 19, 43, fg_color, bg_color_gradient);
  create_texture_row_tube(artery_tree_texture, "|", 20, 43, fg_color, bg_color_gradient);
  create_texture_row_tube(artery_tree_texture, "|", 21, 43, fg_color, bg_color_gradient);
  create_texture_row_tube(artery_tree_texture, "---+", 22, 40, fg_color, bg_color_gradient);
  create_texture_row_tube(artery_tree_texture, "|", 23, 43, fg_color, bg_color_gradient);
  create_texture_row_tube(artery_tree_texture, "|", 24, 43, fg_color, bg_color_gradient);
}

void create_duct_tree(std::vector<AnatomyLineData>& duct_tree_texture)
{
  auto fg_color = Color::LightGray;
  //auto bg_color = Color::DarkMagenta;
  Gradient<Color> bg_color_gradient
  {
    {
      { 0.f, Color::Yellow },
      //{ 0.4f, Color::Yellow },
      { coag_sufficient, Color::White },
      { 0.9f, Color::LightGray },
      { 0.95f, Color::DarkGray },
      { 1.f, Color::Black }
    }
  };
  
  create_texture_row_tube(duct_tree_texture, "|", 19, 45, fg_color, bg_color_gradient);
  create_texture_row_tube(duct_tree_texture, "|", 20, 45, fg_color, bg_color_gradient);
  create_texture_row_tube(duct_tree_texture, "|", 21, 45, fg_color, bg_color_gradient);
  create_texture_row_tube(duct_tree_texture, "----+", 22, 41, fg_color, bg_color_gradient);
  create_texture_row_tube(duct_tree_texture, "|", 23, 45, fg_color, bg_color_gradient);
  create_texture_row_tube(duct_tree_texture, "|", 24, 45, fg_color, bg_color_gradient);
}

//   00000000001111111111222222222233333333334444444444555555555566666666667777777777
//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
//00 +------------------------------------------------------------------------------+
//01 |                                                                              |
//02 |                                                                              |
//03 |                                                                              |
//04 |                                      ###############                         |
//05 |                                  ##########################                  |
//06 |                              ###########ooooooooo#################           |
//07 |                           #############oOOOOOOOOOo######################     |
//08 |                        ###############oOOOOOOOOOOo###########################|
//09 |                      #################oOOOOOOOOOOo###########################|
//10 |                    ###################oOOOOOOOOOo############################|
//11 |                  #####################oOOOOOOOOOo############################|
//12 |                ########################oOOOOOOOo#############################|
//13 |              ##########################oOOOOOOOo#############################|
//14 |             ###########################oOOOOOOo##############################|
//15 |            ############################oOOOOOOo##############################|
//16 |           #############################oOOOOOo###############################|
//17 |          ##############################oOOOOoo###############################|
//18 |         ################################ooooo################################|
//19 |        ###############################/  | | \###############################|
//20 |       ##############################(    | |   )#############################|
//21 |      #############################(    --|-+      )##########################|
//22 |    ############################(      ---+ |         )#####################  |
//23 |              #################(          | |            )###########         |
//24 |                                \_________|_|___________/                     |
//25 |                                                                              |
//26 |                                                                              |
//27 |                                                                              |
//28 |                                                                              |
//29 +------------------------------------------------------------------------------+

void create_hepatoduodenal_ligament(std::vector<AnatomyLineData>& hd_ligament_texture)
{
  auto fg_color = Color::Yellow;
  auto fg_cut_color = Color::Yellow;
  Gradient<Color> bg_color_gradient
  {
    {
      { 0.f, Color::DarkYellow },
      { 0.5f, Color::Yellow },
      { coag_sufficient, Color::White },
      { 0.9f, Color::LightGray },
      { 0.95f, Color::DarkGray },
      { 1.f, Color::Black }
    }
  };
  
  create_texture_row_uniform(hd_ligament_texture, '/', ' ', '\\', 19, 40, 47, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(hd_ligament_texture, '(', ' ', ')', 20, 38, 49, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(hd_ligament_texture, '(', ' ', ')', 21, 36, 52, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(hd_ligament_texture, '(', ' ', ')', 22, 33, 55, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(hd_ligament_texture, '(', ' ', ')', 23, 32, 58, fg_color, fg_cut_color, bg_color_gradient);
  create_texture_row_uniform(hd_ligament_texture, '\\', '_', '/', 24, 33, 57, fg_color, fg_cut_color, bg_color_gradient);
}

template<int NR, int NC>
void draw_ground(SpriteHandler<NR, NC>& sh)
{
  auto fg_color = Color::DarkYellow;
  auto bg_color = Color::Magenta;
  for (int r = 21; r < 29; ++r)
    sh.write_buffer(str::rep_char('#', 78), r, 1, fg_color, bg_color);
}
