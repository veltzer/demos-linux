#ifndef __CLIENTCONTEXT_H_
#define __CLIENTCONTEXT_H_

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"

typedef ACE_Hash_Map_Manager<const char *, void *, ACE_Null_Mutex>
Map;

// Client-specific context information.
class ClientContext
{
public:
  void *get_attribute (const char *name);
  void set_attribute (const char *name, void *value);

private:
  Map attributeMap_;
};
#endif /* __CLIENTCONTEXT_H_ */
