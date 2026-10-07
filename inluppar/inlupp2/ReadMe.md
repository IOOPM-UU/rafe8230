2.1.1 Add Merchandise
This adds a new merch to the warehouse with a name (string), description (string), and price (integer).
A newly added merch is not in stock.
Adding a new merch with the same name as an existing merch is not allowed.
The action code shall be "A". The action shall read the name, description and price for the merch.
2.1.2 List Merchandise
This should list all items in the store. Items should preferably (soft requirement) be printed in alphabetical order on their names. Because there may be more things in the database than might fit on a screen, items should be printed 20 at a time, and the user is asked to continue listing (if possible) or return to the main menu.
The action code shall be "L". The question to continue listing shall return to the main menu if the input begins with "N" or "n". For all other input (including an empty line) the listing shall continue.
2.1.3 Remove Merchandise
Removes an item completely from the warehouse, including all its stock.
The action code shall be "D". The action shall read the name of the merch and a confirmation string. The merch shall be removed only if the confirmation string begins with "Y" or "y".
2.1.4 Edit Merchandise
Allows changing the name, description and price of a merch. Note that this does not affect its stock.
Changing the name of a merch to the name of an existing merch is not allowed..
Note that changing the name may mean changing the key unless you use a unique id for each merch.
The action code shall be "E". The action shall read the old name of the merch, the new name, the new description, the new price for the merch and a confirmation string. The merch shall be updated only if the confirmation string begins with "Y" or "y".
2.1.5 Show Stock
List all the storage locations for a particular merch, along with the quantities stored on each location. Storage locations should preferably be listed in alphabetical order (e.g., A20 before B01 and C01 before C10).
Names of storage locations follow this format always: one capital letter (A-Z) followed by two digits (0-9).
The action code shall be "S. The action shall read the name of the merch.
2.1.6 Replenish
Increases the stock of a merch by at least one.
You can replenish on an existing storage location or a new one.
The stock for a merch is the sum of all items on all storage locations holding that merch.
A storage location stocks items of one (type of) merch, never more.
For simplicity, there is no limit to the amount of storage locations nor is there a limit on the number of items a location can hold.
The action code shall be "P". The action shall read the id of the storage location, the name of the merch and the number of items to add.
2.1.7 Create Cart
Creates a new shopping cart in the system which is empty.
A shopping cart represents a possible order.
Adding/removing merch to/from a cart does not change the stock for that merch – stocks are changed only during checkout.
Shopping carts are identified by a monotonically increasing number, i.e., the number of the i’th shopping cart created is i, regardless of how many shopping carts have been removed.
The action code shall be "C".
2.1.8 Remove Cart
Removes a shopping cart from the system.
The action code shall be "R". The action shall read the id of the cart and a confirmation string. The cart shall be removed only if the confirmation string begins with "Y" or "y".
2.1.9 Add To Cart
Adds some quantity of a merch to a specific shopping cart.
All possible orders in the system must be fullfillable. For example, we may only have one or more carts with 12 items of a merch X if the total stock of X in the system at least 12. Thus, if all users go to checkout at the same time, they should all succeed.
The action code shall be "+". The action shall read the id of the cart, the name of the merch and the number of items.
2.1.10 Remove From Cart
Removes zero or more items of some merch from a particular cart.
The action code shall be "-". The action shall read the id of the cart, the name of the merch and the number of items.
2.1.11 Calculate Cost
Calculate the cost of a shopping cart. If a cart holds 2 items of a merch M1 with a price of 50 and 8 items of a merch M2 with a price of 3 the cost of the cart is 2×50+8×3=124
The action code shall be "=". The action shall read the id of the cart.
2.1.12 Checkout
This action represents the user going through with a purchase of all the items in a particular shopping cart.
Decrease the stock for the merches in the cart.
Remove the shopping cart from the system.
The action code shall be "O". The action shall read the id of the cart.
2.1.13 Undo [optional]
Undos an action.
Multiple undos should be supported (i.e., pressing undo N times undos the N last actions for N≤16.)
You cannot “undo an undo” (aka “redo”).
The action code shall be "U". 
2.1.14 Quit
Quits the program.
The action code shall be "Q".
The action shall read a confirmation string. The program shall exit only if the confirmation string begins with "Y" or "y"
2.1.15 Persistance on File [optional]
The program should save its data on disk between runs. There is a discussion about how to implement this further down in this document.

## Generic hash table

The generic hash table uses one combined implementation: a dynamically resized array of bucket pointers, with linked chains for collisions. It grows and shrinks as entries are inserted and removed. From this directory, run `make -f MakeFile test` to build and run its tests.

The public API is declared in `generic_data_structs/hash_table.h`.