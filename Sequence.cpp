// FILE: Sequence.cpp
// CLASS IMPLEMENTED: sequence (see sequence.h for documentation)
// INVARIANT for the sequence ADT:
//   1. The number of items in the sequence is in the member variable
//      used;
//   2. The actual items of the sequence are stored in a partially
//      filled array. The array is a dynamic array, pointed to by
//      the member variable data. For an empty sequence, we do not
//      care what is stored in any of data; for a non-empty sequence
//      the items in the sequence are stored in data[0] through
//      data[used-1], and we don't care what's in the rest of data.
//   3. The size of the dynamic array is in the member variable
//      capacity.
//   4. The index of the current item is in the member variable
//      current_index. If there is no valid current item, then
//      current_index will be set to the same number as used.
//      NOTE: Setting current_index to be the same as used to
//            indicate "no current item exists" is a good choice
//            for at least the following reasons:
//            (a) For a non-empty sequence, used is non-zero and
//                a current_index equal to used indexes an element
//                that is (just) outside the valid range. This
//                gives us a simple and useful way to indicate
//                whether the sequence has a current item or not:
//                a current_index in the valid range indicates
//                that there's a current item, and a current_index
//                outside the valid range indicates otherwise.
//            (b) The rule remains applicable for an empty sequence,
//                where used is zero: there can't be any current
//                item in an empty sequence, so we set current_index
//                to zero (= used), which is (sort of just) outside
//                the valid range (no index is valid in this case).
//            (c) It simplifies the logic for implementing the
//                advance function: when the precondition is met
//                (sequence has a current item), simply incrementing
//                the current_index takes care of fulfilling the
//                postcondition for the function for both of the two
//                possible scenarios (current item is and is not the
//                last item in the sequence).

#include <cassert>
#include "Sequence.h"
#include <iostream>
using namespace std;

namespace CS3358_Sp2016
{
   // CONSTRUCTORS and DESTRUCTOR
   sequence::sequence(size_type initial_capacity)
       : used(0), current_index(0), capacity(initial_capacity)
   {
       // Validate capacity
       if (capacity < 1) {
           capacity = 1;
       }

       data = new value_type[capacity];
   }

   sequence::sequence(const sequence& source)
       : used(source.used), current_index(source.current_index),
         capacity(source.capacity)
   {
       data = new value_type[capacity];
       for (int i = 0; i < used; i++) {
           data[i] = source.data[i];
       }
   }

    sequence::~sequence()
    {
        delete [] data;
    }

    // MODIFICATION MEMBER FUNCTIONS
    void sequence::resize(size_type new_capacity)
    {
        // Ensure new_capacity will be able to retain existing data
        if (new_capacity < 1 || new_capacity < used) {
            // Set capacity to exactly what is needed or to 1
            capacity = used > 0 ? used : 1;
        } else {
            // If the capacity is growing
            capacity = (new_capacity == capacity) ? new_capacity + 1 : new_capacity;

            value_type *temp = new value_type[capacity]; // Allocate space
            if (temp == NULL) // Ensure memory is allocated
            {
                cerr << "*** Error: Memory not available. Exiting program. ***";
                exit(0);
            }

            for (int i = 0; i < used; i++) {
                temp[i] = data[i]; // Copy existing values
            }

            delete [] data; // Free up old space
            data = temp; // Update sequence::data to new memory location
        }
    }

    void sequence::start() {
        // The first item on the sequence becomes the current item
        // (but if the sequence is empty, then there is no current item).
        if (used > 0) {
            current_index = 0;
        }
    }

    void sequence::advance()
    {
        // If item(s) exists in sequence
        if (is_item()) {
            // Increment the current_index if there is a current item
            if (current_index != used) {
                current_index++;
            }
        }
    }

    void sequence::insert(const value_type& entry)
    {
        if (used + 1 > capacity) { // If adding to sequence will exceed capacity
            cout << "RESIZING" << endl;
            resize((int) capacity * 1.25);
        }

        cout << "CAPACITY: " << capacity << endl;
        if (current_index == used && used == 0) // If there is no current index
        { // and the sequence is empty
            data[0] = entry; // insert entry into first position
            used++;
            return; // Our work here is done
        }

        if (current_index == used && used > 0) { // If there is no current index
            current_index = 0; // and the sequence is not empty, update current_index
        }

        // Copy current data into new sequence leaving space for new entry
        value_type *temp = new value_type[capacity]; // Allocate space
        if (temp == NULL) // Ensure memory is allocated
        {
            cerr << "*** Error: Memory not available. Exiting program. ***";
            exit(0);
        }

        int offSet = 0;
        for (int i = 0; i < used; ++i) {
            cout << "Copy " << data[i] << " into index " << i << endl;
            if (current_index == i) {
                offSet = 1;
            }
            temp[i+offSet] = data[i]; // Copy existing values
        }

        delete [] data; // Free up old space
        data = temp; // Update numbers to new memory location

        data[current_index] = entry; // Write the entry to the current_index
        used++;
    }

    void sequence::attach(const value_type& entry) {
//    Post: A new copy of entry has been inserted in the sequence after
//      the current item. If there was no current item, then the new
//      entry has been attached to the end of the sequence. In either
//      case, the newly inserted item is now the current item of the
//      sequence.

        if (used + 1 > capacity) { // If adding to sequence will exceed capacity
            cout << "RESIZING" << endl;
            resize((int) capacity * 1.25);
        }

        if (current_index == used) // If there is no current index
        {
            data[used] = entry; // insert entry into first position
            current_index = used;
            used++;
            return; // Our work here is done
        }

        // Copy current data into new sequence leaving space for new entry
        value_type *temp = new value_type[capacity]; // Allocate space
        if (temp == NULL) // Ensure memory is allocated
        {
            cerr << "*** Error: Memory not available. Exiting program. ***";
            exit(0);
        }

        int offSet = 0;
        for (int i = 0; i < used; ++i) {
            cout << "Copy " << data[i] << " into index " << i << endl;
            if (current_index+1 == i) {
                offSet = 1;
            }
            temp[i+offSet] = data[i]; // Copy existing values
        }

        delete [] data; // Free up old space
        data = temp; // Update numbers to new memory location

        data[current_index+1] = entry; // Write the entry to the current_index
        current_index++;
        used++;
    }

    void sequence::remove_current()
    {
        cout << "remove_current() not implemented yet" << endl;
    }

    sequence& sequence::operator=(const sequence& source)
    {
        cout << "operator=(const sequence& source) not implemented yet" << endl;
        return *this;
    }

   // CONSTANT MEMBER FUNCTIONS

    sequence::size_type sequence::size() const
    {
        return used;
    }

    bool sequence::is_item() const
    {
        // There can only be a valid current item if used > 0
        if (current_index != used) {
            return true;
        }

        return false;
    }

    sequence::value_type sequence::current() const
    {
        // If item(s) exists:
        if (is_item()) {
            return data[current_index];
        }

        return value_type();
    }
}

