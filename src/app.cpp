#include "app.h"

// when something goes wrong inside lua
// this reports the error message through the standard output
void report_errors(lua_State *L, int status) {
  if ( status!=0 ) {
    std::cout << "-- " << lua_tostring(L, -1) << std::endl; // could use cerr instead of cout
    lua_pop(L, 1); // remove error message
  }
}


// 1. starting and closing the lua VM
/*
int main(int argc, char* argv[]) {

  std::cout << "starting App : lua tutorial" << std::endl;

  std::cout << "starting lua virtual machine" << std::endl;
  lua_State *L = lua_open();
  std::cout << "closing lua virtual machine" << std::endl;
  lua_close(L);
  return 0;
}*/


// 2. calling lua programs inside c++ code
/*int main(int argc, char** argv)
{
  for ( int n=1; n<argc; ++n ) { // parse the arguments and loop on them
    const char* file = argv[n]; // each argument is a lua code file to be executed

    lua_State *L = lua_open(); // starts the lua VM

    luaL_openlibs(L); // open some basic libs for lua (io, containers, string manipulation...)

    std::cout << "-- Loading file: " << file << std::endl;

    int s = luaL_loadfile(L, file); // loading a lua code file given in argument

    if ( s==0 ) { // if the load of the file went correctly
      // execute Lua program
      s = lua_pcall(L, 0, LUA_MULTRET, 0);
    }

    report_errors(L, s); // display error messages
    lua_close(L); // close then VM
    std::cout << std::endl;
  }

  return 0;
}*/


// 3. calling a lua function from c++ code, by passing it parameters and retrieving its result
/*
// 3.a. binding of the lua function in c++ 
int luaadd (lua_State* L, int x, int y ) {
  int sum;

  // the function name 
  lua_getglobal(L, "add");

  // the first argument 
  lua_pushnumber(L, x);

  // the second argument 
  lua_pushnumber(L, y);

  // call the function with 2 arguments, return 1 result 
  lua_call(L, 2, 1);

  // get the result 
  sum = (int)lua_tointeger(L, -1);
  lua_pop(L, 1);

  return sum;
}

// 3.b. using our function in main
int main(int argc, char** argv) {
  const char* file = "../../scripts/add.lua"; // the lua code file containing the lua addition function

  lua_State *L = lua_open(); // starts the lua VM

  luaL_openlibs(L); // open some basic libs for lua (io, containers, string manipulation...)

  std::cout << "-- Loading file: " << file << std::endl;

  luaL_dofile(L, file); // load and execute add.lua, now we can use the add function that was inside add.lua

  // call our binding of the add function that is in add.lua
  std::cout << luaadd(L, 2, 3) << std::endl;

  lua_close(L); // close then VM
  std::cout << std::endl;

  return 0;
}*/


// 4. recover a global variable from lua, changing its value in lua
/*
int main(int argc, char** argv) {

  const char* file="../../scripts/globalvar.lua"; // the lua code file containing a global variable
  lua_State *L = lua_open(); // starts the lua VM
  luaL_dofile(L, file); // read and process the globalvar.lua file
  lua_getglobal(L, "z"); // put the lua global variable z on the stack
  int z = (int)lua_tointeger(L, -1); // converts the variable to an integer an set it to the c++ z local variable
  lua_pop(L, 1); // remove the variable from the stack

  std::cout << "extracted z variable from lua: " << z << std::endl;
  // now we will set the global variable z to 10 in lua : 
  lua_pushnumber(L, 10); // push the number 10 on the stack
  lua_setglobal(L, "z"); // assign it to z in lua

  // we retrieve it like before
  lua_getglobal(L, "z"); // put the lua global variable z on the stack
  z = (int)lua_tointeger(L, -1); // converts the variable to an integer an set it to the c++ z local variable
  lua_pop(L, 1); // remove the variable from the stack


  std::cout << "extracted z variable from lua, after reasignment: " << z << std::endl;

  return 0;
}*/

// 5. calling c++ functions from lua

// a C++ function that we want to call from lua :
static int average(lua_State *L) {
  // get number of arguments 
  // when calling a function, the first element of the lua stack if the number of parameters
  int n = lua_gettop(L); // so this is the number of params given to the lua function in the lua script
  double sum = 0;

  // after that, the following elements of the lua stack are the function parameters (from rank 1 to n)
  // loop through each argument 
  for (int i = 1; i <= n; i++) {
    // total the arguments 
    sum += lua_tonumber(L, i); // we recover each of this parameters as c++ doubles and add it to the c++ variable sum
  }

  // then we push the results in the lua stack to recover them inside the lua script
  // push the average 
  lua_pushnumber(L, sum / n); 

  // push the sum 
  lua_pushnumber(L, sum);

  // return the number of results 
  return 2;
}



int main ( int argc, char *argv[] )
{
  // initialize Lua 
  lua_State *L = lua_open();

  // load Lua base libraries 
  luaL_openlibs(L);

  // register our c++ function inside lua, now we can call it from a lua script
  lua_register(L, "average", average);

  // run the script where we call our c++ function
  luaL_dofile(L, "../../scripts/avg.lua");

  // cleanup Lua 
  lua_close(L);

  // pause 
  printf( "Press enter to exit..." );
  getchar();

  return 0;
}
