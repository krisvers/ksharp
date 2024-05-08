Source:
| -------- |
var: u32 = 2;
fib: (n: u32) u32 = {

}

main: () s32;
| -------- |


Parser AST:
| -------- |
Declaration: "var"
  Identifier: "var"
  Type: "u32"
  Literal Integer: "2"
Function Declaration: ""
  Identifier: "fib"
  Return Type: "u32"
  Scope: "{"
  Scope End: "}"
Function Declaration: ""
  Identifier: "main"
  Return Type: "s32"
| -------- |

C Code Gen:
#include <stdint.h>
uint32_t ksharp_var = 2;
uint32_t ksharp_fib(){
}

int32_t ksharp_main();

int main(int argc, char** argv) {
	return ksharp_main();
}
