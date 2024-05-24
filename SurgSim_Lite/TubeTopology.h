#pragma once
#include "../../lib/Termin8or/RC.h"
#include <vector>

namespace tube_topology
{
  // Artery Topology (* : root).
  // Test flow by checking for inactive pixels and then walk towards clip (if any), thus towards source.
  //    0 : 0(1)
  //    1 : 1(2)
  //    2 : 2(5)
  //3 4 5 : 3(4), 4(5), 5(6)
  //    6 : 6(7)
  //    7*: 7(null)
  
  // Duct Topology (* : root)
  //    0*: 0(null)
  //    1 : 1(0)
  //    2 : 2(1)
  //3 4 5 : 3(4), 4(5), 5(2)
  //    6 : 6(5)
  //    7 : 7(6)
  
  struct TopologyElem
  {
    RC curr;
    TopologyElem* next = nullptr; // Towards source.
  };
  
  std::vector<TopologyElem> artery_tree_topology;
  std::vector<TopologyElem> duct_tree_topology;
  
  void create_topologies()
  {
    //"|", 19, 43, fg
    //"|", 20, 43, fg
    //"|", 21, 43, fg
    //"---+", 22, 40,
    //"|", 23, 43, fg
    //"|", 24, 43, fg
    
    artery_tree_topology.resize(9);
    artery_tree_topology[0].curr = { 19, 43 };
    artery_tree_topology[0].next = &artery_tree_topology[1];
    artery_tree_topology[1].curr = { 20, 43 };
    artery_tree_topology[1].next = &artery_tree_topology[2];
    artery_tree_topology[2].curr = { 21, 43 };
    artery_tree_topology[2].next = &artery_tree_topology[3];
    artery_tree_topology[3].curr = { 22, 43 };
    artery_tree_topology[3].next = &artery_tree_topology[4];
    artery_tree_topology[4].curr = { 23, 43 };
    artery_tree_topology[4].next = &artery_tree_topology[5];
    artery_tree_topology[5].curr = { 24, 43 };
    artery_tree_topology[5].next = nullptr;
    artery_tree_topology[6].curr = { 22, 40 };
    artery_tree_topology[6].next = &artery_tree_topology[7];
    artery_tree_topology[7].curr = { 22, 41 };
    artery_tree_topology[7].next = &artery_tree_topology[8];
    artery_tree_topology[8].curr = { 22, 42 };
    artery_tree_topology[8].next = &artery_tree_topology[3];
    
    
    //"|", 19, 45, fg_
    //"|", 20, 45, fg_
    //"|", 21, 45, fg_
    //"----+", 22, 41,
    //"|", 23, 45, fg_
    //"|", 24, 45, fg_
    
    duct_tree_topology.resize(10);
    duct_tree_topology[0].curr = { 24, 45 };
    duct_tree_topology[0].next = &duct_tree_topology[1];
    duct_tree_topology[1].curr = { 23, 45 };
    duct_tree_topology[1].next = &duct_tree_topology[2];
    duct_tree_topology[2].curr = { 22, 45 };
    duct_tree_topology[2].next = &duct_tree_topology[3];
    duct_tree_topology[3].curr = { 21, 45 };
    duct_tree_topology[3].next = &duct_tree_topology[4];
    duct_tree_topology[4].curr = { 20, 45 };
    duct_tree_topology[4].next = &duct_tree_topology[5];
    duct_tree_topology[5].curr = { 19, 45 };
    duct_tree_topology[5].next = nullptr;
    duct_tree_topology[6].curr = { 22, 41 };
    duct_tree_topology[6].next = &duct_tree_topology[7];
    duct_tree_topology[7].curr = { 22, 42 };
    duct_tree_topology[7].next = &duct_tree_topology[8];
    duct_tree_topology[8].curr = { 22, 43 };
    duct_tree_topology[8].next = &duct_tree_topology[9];
    duct_tree_topology[9].curr = { 22, 44 };
    duct_tree_topology[9].next = &duct_tree_topology[2];
  }

  int find_tube_topology_pixel(const RC& rc, TextureType tex_type)
  {
    switch (tex_type) {
      case TextureType::ARTERY_TREE:
      {
        auto it = std::find_if(artery_tree_topology.begin(), artery_tree_topology.end(),
          [rc](const auto& elem)
          {
            return elem.curr == rc;
          });
        return static_cast<int>(std::distance(artery_tree_topology.begin(), it));
        break;
      }
      case TextureType::DUCT_TREE:
      {
        auto it = std::find_if(duct_tree_topology.begin(), duct_tree_topology.end(),
          [rc](const auto& elem)
          {
            return elem.curr == rc;
          });
        return static_cast<int>(std::distance(duct_tree_topology.begin(), it));
        break;
      }
      default:
        break;
    }
    return -1;
  }
}
