#ifndef MAGEQUEST_SRC_QUEST_SCRIPTEDNODES_H_
#define MAGEQUEST_SRC_QUEST_SCRIPTEDNODES_H_

//Persistent info to allow for scripting inside function
struct ScriptedNodeInfo {
  bool bools[5] = {false};
  int ints[5] = {0};
  void* ptrs[5] = {nullptr};
};

//Function typedef
typedef bool (*FunctionPointer)(ScriptedNodeInfo&);

bool StoneKnightFightWithAria(ScriptedNodeInfo&) {
  return true;
}


std::array<FunctionPointer, 1> functionArray{StoneKnightFightWithAria};

#endif  //MAGEQUEST_SRC_QUEST_SCRIPTEDNODES_H_
