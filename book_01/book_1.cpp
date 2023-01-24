
/*********************************************************************************************************
book_1: "The C++ Standard Library: A Tutorial and Reference"

notes:
- p. 11: amortized complexity / amortized cost
         how operation behaves in the long run / considering a very long sequence
         e.g.: append element to dynamic array
               => if enough memory, then O(1)
               => otherwise, have to (re)allocate new memory, then linear O(N_elements) + O(allocation)
                  => have to allocate new memory and copy all elements into new memory
         e.g.: std::vector:
               => as long as capacity not exceeded, push_back should be O(1)
               => if capacity would be exceeded, reallocation happens, with at least O(N_elements)
- p. 11: complexity
            O(1)           constant
            O(log(n))      logarithmic (or: log-n)
            O(n)           linear
            O(n·log(n))    n-log-n
            O(n²)          quadratic
- p. 15: uniform initialization, using braces {...}
         narrowing initialization/conversion
- p. 16: std::initializer_list
         "When there are ctors for both a speciﬁc number of arguments and an initializer list,
          the version with the initializer list is preferred"
         explicit ctor to disable implicit type conversions
- p. 17: generic function to print all elements of a collection
- p. 18: range-based for loop equivalent
- p. 20: std::move, rvalue-refs T&&
- p. 21: move-ctor
- p. 22: overloading rules for lvalue- and rvalue-references
         provide only move-semantics: used by unique-ptrs, file-streams, string-streams
         overloading rules for rvalue- and lvalue-refs:
            f(X& x)        => can be called for lvalues only
            f(const X& x)  => can be called for rvalues and lvalues
            f(X&& x)       => can be called for rvalues only
            -- to distinguish dealing with rvalues and lvalues, provide the following 2 implementions:
               f(X& x)
               f(X&& x)
                  or
               f(const X& x)
               f(X&& x)
         shouldn't std::move return values
- p. 27: alias template (template typedef)
- p. 30: declare lambda as mutable, to pass in variable by value, but be able to change it inside the lambda
         => value is retained across mutliple calls => this is like a static variable of a function for a lambda (or a functor)
- p. 37: initialize to 0 in template, for any type T: 'T x = T();' (or 'T x{};' ?)
- p. 40: using declaration, using directive
- p. 44: all headers related to exceptions
- p. 53: pass exception between threads
- p. 66: std::pair<int,float>(42,77.7) != std::make_pair(42,77.7) // (int,float) vs. (int,double)
         to force reference semantics for pair/tuple, have to use std::ref(arg) or std::cref(arg)
- p. 67: when comparing pairs, 1st value has higher prio
            x.first < y.first || (!(y.first < x.first) && x.second < y.second)

               yields:
                  if x.first < y.first:
                     // true || (?)
                     return true
                  else:
                     // false || (?), x.first >= y.first
                     if !(y.first < x.first):
                        // x.first >= y.first && y.first >= x.first => y.first == x.first
                        // false || (true && (?))
                        if x.second < y.second:
                           // false || (true && true)
                           return true
                        else:
                           // false || (true && false)
                           return false
                     else:
                        // false || false
                        return false

            assume x < y:
               x.first < y.first == true           // true || (?) => return true
            assume x > y:
               x.first < y.first == false          // false || (?)
                  !(y.first < x.first) == false    // false || (false && (?)) => return false
            assume x == y:
               x.first < y.first == false          // false || (?)
                  !(y.first < x.first) == true     // false || (true && (?))
                     x.second < y.second == ?      // false || (true && (?))
            
         note: when only "operator<" is available, then
            v1<v2                  => v1<v2
            v1>v2                  => v2<v1
            v1<=v2                 => !(v2<v1)
            v1>=v2                 => !(v1<v2)
            !(v1<v2) && !(v2<v1)   => v1==v2    # v1>=v2 && v1<=v2 => v1==v2
- p. 72: std::tie to create a tuple of references (no longer needed in C++17, as we have structured binding)
- p. 77/80: shared_ptr
         default deleter provided by shared_ptr calls 'delete', not 'delete[]' !!!
         => so if I allocate array, HAVE TO define own deleter !!! (or use 'std::default_delete<int[]>()')
- p. 83: shared memory, shared_ptr, own deleter as functor
- p. 84: shared_ptr only provide * and ->, no pointer arithmetic and [] -- to access memory, use get()
- p. 85: weak_ptr allows sharing without owning an object -- requires a shared_ptr to get created
         can't use * and -> to access referenced object -- have to create a shared_ptr out of the weak_ptr
- p. 87: use weak_ptr  to break cycle of shared_ptrs, so resources get released as desired
- p. 90: only one shared_ptr can own the managed object! otherwise multi-free!
- p. 92: shared_ptr
         "Whenever ownership is transferred to a shared pointer that already owned another object,
         the deleter for the previously owned object gets called if that shared pointer was the last owner.
         The same applies if a shared pointer gets a new value either by assigning a new value
         or by calling reset():
         If the shared pointer previously owned an object and was the last owner,
         the corresponding deleter (or delete) gets called for the object."
- p. 93: shared_ptr
         shared_ptr<T> sp(ptr)         // creates a shared pointer owning *ptr
         shared_ptr<T> sp(sp2)         // creates a shared pointer sharing ownership with sp2
         shared_ptr<T> sp(move(sp2))   // creates a shared pointer owning the pointer previously owned by sp2 (sp2 is empty afterward)
         sp.reset()                    // gives up ownership and reinitializes the shared pointer as being empty
         sp.reset(ptr)                 // gives up ownership and reinitializes the shared pointer owning *ptr
         make_shared(...)              // creates a shared pointer for a new object initialized by the passed arguments
         ...
- p. 95: shared_ptr: always prefer 'make_shared<X>(...)' over 'shared_ptr<X>(new X(...))'
- p. 98: unique_ptr: implements exclusive ownership
- p.100: unique_ptr also provides 'release' to give up ownership (shared_ptr cannot 'release'!)
- p.101: only one unique_ptr can own the managed object! otherwise multi-free!
         you can't copy or assign a unique_ptr, can only move it!
- p.102: unique_ptr: assigning nullptr has the same effect as calling reset()
         up = nullptr;  ==  up.reset();
         unique_ptr as source/sink of data
- p.105: by default, unique_ptrs call delete for an object they own if they lose ownership
         (because they are destroyed, get a new object assigned, or become empty)

         instead of providing custom deleter for unique_ptr to call delete[],
         use partial specialization of class unique_ptr to deal with arrays
            this partial specialization's interface is a little different:
            don't have *, ->, but []
- p.106: unique_ptr: custom deleter
- p.109: using unique_ptr to manage POSIX opendir, readdir, closedir
- p.111: unique_ptr
         unique_ptr<T> up(ptr)         // creates a unique pointer owning *ptr
         unique_ptr<T> up(move(up2))   // creates a unique pointer owning the pointer previously owned by up2 (up2 is empty afterward)
         up.reset()                    // calls deleter for an owned object and makes up empty
                                       // equivalent to 'up=nullptr;'
         up.reset(ptr)                 // calls deleter for an owned object and reinitializes the unique pointer to own *ptr
         up.release()                  // gives up ownership back to the caller (returns owned object without calling the deleter)
         make_unique(...)              // creates a unique pointer for a new object initialized by the passed arguments (C++14)
         ...

- p.112: unique_ptr: reference to deleter possible
            unique_ptr<int, D&> p3(new int, d);    // p3 holds a reference to d
- p.114: on performance of smart-pointers' deleters
         "to avoid the introduction of unnecessary overhead, you should use function objects (including lambdas) for deleters"
         
         "for any smart pointer class, you should never create multiple smart pointers out of the same ordinary pointer"
- p.115: numeric limits: minimum guaranteed precision of types
- p.124: template metaprogramming to provide 2 implementations: one for all integral types, and one for all floating-point types
- p.132: std::ref(), std::cref
         "allows function templates to work on references without specialization"
         to create a pair<> / tuple<> of references
         std::reference_wrapper<> to create STL container of references,
            e.g.: std::vector<std::reference_wrapper<int>> v; // == std::vector<int&> v;
- p.133: std::function to create vector of functions
- p.138: !=, >, <=, and >= defined in terms of == and <
- p.148: split value in ms into into hours, minutes, seconds, and milliseconds
            val_ms = 7255042
            hours     = int( (val_ms/1000.0) / 3600      )   // val_ms/1000.0 is val_in_seconds; 1 hour == 60 min == 60*60s==3600s
            minutes   = int( (val_ms/1000.0) % 3600 / 60 )
            seconds   = int( (val_ms/1000.0) % 60        )
            millisecs = val_ms % 1000
               => cout << "total duration of " << val_ms << " msecs in h::min::sec::msec:" << std::setfill('0')
                       << std::setw(2) << hours << "::"
                       << std::setw(2) << minutes << "::"
                       << std::setw(2) << seconds << "::"
                       << std::setw(2) << millisecs << endl;
                       (should be: 02::00::55::042   // [2 of 3600/1]::[0 of 60/1]::[55 of 1/1]::[42 of 1/1000])
- p.161: NULL/nullptr in C++ vs. C
         "NULL in C++ is guaranteed to be simply the value 0"
         "C++ requires that the type of NULL be an integer type"
- p.165: components of the STL: containers, iterators, function objects, and algorithms
- p.166: STL components figure
- p.168: advantage of associative containers when searching an element
         "The key advantage of automatic sorting is better performance when you search element"
         => can always use binary search which is O(log(n)) instead of O(n)
- p.187: prefer unordered_map over map:
         "because the complexity for element access is amortized constant for unordered maps rather than logarithmic for maps,
         you should usually prefer unordered maps over maps unless you need the sorting"
- p.193: how range-for is interpreted
- p.203: "valid range" (see also p.246)
         end of the range is reachable from the beginning by iterating through the elements
         => both iterator of range belong to same container
         => begin is not behind the end      // begin can be equal end, then range/collection is empty
         a (half-open) range: [begin,end)    // "avoids special handling for empty collections"
- p.205: if not clear which iterator of the 2 forming a range comes first
         1) for random access iterator:
            can use operator<
         2) without random access iterators:
            it1 = ...
            search for it2
               in range [begin, it1) or
               in range [it1, end)
- p.207: calling algorithms with multiple ranges
         => have to make sure the 2nd and additional ranges have at least as many elements as the 1st range
         => have to make sure the destination ranges are big enough for algos that write to collections
- p.210: Both resizing and initializing the size creates new elements. These elements are initialized by their default ctor.
            | std::vector<int> v; | std::vector<int> v(N); |
            | v.resize(n);        |                        |
         note: better option (for algos that take 2 or more ranges)  might be
            std::vector<int> v;
            v.reserve(N);
            // then use 'std::back_inserver(v)'
- p.213: end-of-stream iterator: 'std::istream_iterator<std::string>()'
- p.217: "Iterators are the glue between containers and algorithm"
- p.218: 'remove' doesn't really remove anything from the container. to really remove, have to use 'erase'
         e.g.:
               auto end = remove(coll.begin(), coll.end(), 3); // returns the new logical end of the collection coll after "removing" all elements equal to 3
               coll.erase(end, coll.end());
         or all in one:
               coll.erase( remove(coll.begin(), coll.end(), 3), coll.end() );
- p.221: how to remove elements in associative or unordered containers? => call their member functions
- p.223: prefer member functions (e.g.: coll.remove(3)) over algorithms (e.g: remove(coll.begin(),coll.end(),3))
         e.g.:
            coll.remove(3);
               instead of
            coll.erase(remove(coll.begin(), coll.end(), 3), coll.end());
- p.226: isPrime
- p.228: sort Person by last name and first name (using lambda p.232)
         difference for using predicate:
            function: binary function predicate
            class: functor => used as sorting criterion when declaring sets (p.232)
- p.235: advantages of function object (functor)
            can have state
            can be initialized at runtime before you use/call them
            has its own type (can have different types even when their signatures are the same, unlike ordinary functions)
            usually faster than ordinary functions
- p.236: want to add a certain value to all elements of a collection
            if I know the value to add at compile time:                 => could use ordinary functions
            if I need different values to add known at compile time:    => could use template
            if I want to process the value to add at runtime:           => could use global variable (messy)
                                                                        => could use functor <<--
- p.239: predefined function objects: less<T>, greater<T>, negate<T>, multiplies<T>, equal_to<T>
- p.244: requirements for container elements
            elements must be copyable or movable
            an element must be (move-) assignable 
            an element must be destroyable by a dtor
            + possibly
               default ctor
               operator==
               sorting criterion
               hash function, equivalence criterion
- p.245: reference semantics:
         In C, function arguments are able to get passed only by value, so you need pointers to enable a call-by-reference
         - in C: call-by-value (int i), call-by-reference (int* i)
         - in C: call-by-value (int i), call-by-reference (int* i), call-by-reference-param (int& i)
         implement reference semantics (on top of value semantics) in C++
         - shared_ptr
         - reference_wrapper
- p.246: iterators become invalid
         - for vectors & deques, if elements are inserted or deleted or reallocation takes place
         - for unordered containers, if rehashing takes place (which also might be a result of an insertion)
- p.246: "valid range" (see also p.203)
         - both iterators refer to the same container
         - the 2nd iterator must be reachable from the 1st iterator
         - "If more than one source range is used, the 2nd and later ranges usually must have at least as many elements as the 1st one."
         - "Destination ranges must have enough elements that can be overwritten; otherwise, insert iterators must be used."
- p.247: gcc provides '-D_GLIBCXX_DEBUG' for STL safe mode
- p.251: "To add new behavior for containers, you should deﬁne a new class that internally uses STL classes
          or derives privately from them"
- p.255: required operations for STL containers
- p.258: swap and invalidation of iterators
         "Iterators and references to elements of a container follow swapped elements.
          So, after swap(), iterators and references still refer to the elements they referred to before,
          which, however, are in a different container then.""
         p.263: for std::array
            iterators and references don’t swap containers with their elements,
            so iterators and references refer to the same container but different elements afterward
         p.267: for std::array
            Iterators remain valid as long as the array remains valid. However, unlike for all other containers,
            swap() assigns new values to the elements that iterators, references, and pointers refer to.
- p.259: for unordered containers, the operators <, <=, >,  >= are NOT provided, only == and !=
- p.267: std::array: as contiguous memory, this is true:
            &a[i] == &a[0] + i
            &*(a+i) == &*a + i
               int a[10]; int i = 2;
               cout << (&a[i]) << "\n" << (&a[0]+i) << "\n" << (&*(a+i)) << "\n" << (&*a+i) << "\n";
         if array is need for C library, can use std::array<> or std::vector<> and a.data() for C-style interface
- p.271: std::vector: to avoid reallocation, call reserve(), or initialize with enough elements
            std::vector<int> v; | std::vector<T> v(50); // calls 50 times default ctor of T
            v.reserve(50);      | 
         => if the only reason for initialization is to reserve memory, use reserve()
         Calling reserve() with an argument that is less than the current capacity is a no-op.
- p.276: std::vector
         Regarding performance, you should consider that inserting and removing happens faster when
         - Elements are inserted or removed at the end.
         - The capacity is large enough on entry.
         - Multiple elements are inserted by a single call rather than by multiple calls
- p.276: std::vector
         - Vectors provide no operation to remove elements directly that have a certain value. Must use an algorithm to do this.
         - helper functions to erase all elements/an element from a vector (p.276,277)
            
            //// remove all elements with value 'val' ////
            std::vector<Elem> v;
            v.erase(std::remove(v.begin(), v.end(), val), v.end());
            
            //// remove only the 1st element that has a certain value 'val' ////
            std::vector<Elem> v;
            auto pos = std::find(v.begin(), v.end(), val);
            if (pos != v.end())
               v.erase(pos)
- p.281: std::vector<bool>: bitfield with dynamic size, std::bitset<T>: bitfield with static size
         c[idx] returns proxy, not a bool
- p.284: std::deque
         - deques provide no support to control the capacity and the moment of reallocation
         - when to prefer deque over vector (see p.284)
- p.285: how deques differ from vectors
         - deque: no capacity() and reserve()
         - deque: direct functions to insert and to delete the 1st element (push_front(), pop_front())
         so:
           | std::vector    | std::deque      |
           +————————————————+—————————————————+
           |    push_back() |    push_back()  |
           |    pop_back()  |    pop_back()   |
           +————————————————+—————————————————+
           |       -        |    push_front() |
           |       -        |    pop_front()  |
           +————————————————+—————————————————+
- p.286: std::deque
         An insertion or deletion of elements might cause reallocation,
         thus, any insertion or deletion invalidates all pointers, references and iterators
         that refer to other elements of the deque.
         The exception is when elements are inserted at the front or the back.
         In this case, references and pointers to elements stay valid, but iterators don't.
- p.291: std::list
         - Inserting and deleting elements does not invalidate pointers, references, and iterators to other elements.
- p.291: how lists differ from arrays, vectors, deques
- p.294: for list, call member functions l.remove() / l.remove_if instead of algorithm std::remove() / std::remove_if()
            
            //// remove all elements with value 'val' ////
            std::list<Elem> l;
            l.remove(val);
            
            //// remove only the 1st element that has a certain value 'val' ////
            std::list<Elem> l;
            auto pos = std::find(l.begin(), l.end(), val);
            if (pos != l.end())
               l.erase(pos)
- p.296: std::list
         The following operations do not invalidate iterators and references to other members:
            insert(), emplace(), emplace...(), push_...(), pop_...(), erase()
- p.300: limitations of forward_list compared to list
         - all member functions that modify forward_list in a way that elements are inserted or deleted
           at a specific position, special versions are provided:
            - these member functions have to pass the position of the preceding element
               <func>_after, e.g.: insert_after()
            - this is why they also provide
               before_begin(), cbefore_begin()
              which can be used to let built-in algorithms ending with _after exchange even the first element
- p.301: std::forward_list
         - Inserting and deleting elements does not invalidate iterators, references, and pointers to other elements.
         - when element positions are involved, you have to pass the preceding position,
           and the member function has the sufﬁx _after in its name
- p.307: std::forward_list
         - find and remove or insert:
            have to find an element by determining whether the **next** elements fits a specific criterion
- p.308: std::forward_list
            own algorithms to find a position before the element that has a specific value or fulfills a specific condition
               p.308: find_before
               p.308: find_before_if
- p.315: def.: strict weak ordering
- p.316: std::set / std::multiset
         - Cannot change the value of an element directly.
           To modify the value of an element, must remove the element having the old value
           and insert a new element with the new value.
         - 2 ways of defining a sorting criterion
         - check for equivalence:
            !( e1<e2 || e2<e1 )  // for '!(e1<e2) && !(e2<e1)', which is less efficient as 2 comparisions have to be evaluated
            => !(A || B) == !A && !B
- p.317: std::set / std::multiset
         - advantages of only using sorting criterion for equivalence checking
            - only 1 argument to pass as sorting criterion
            - element type not forced to have operator==
            - contrary definitions between equivalence & equality possible (but might be confusing)
- p.319: lower_bound, upper_bound; equal_range := [lower_bound, upper_bound)
      if lower_bound == upper_bound:
         no elements with the same value exist in the set or multiset

      notes:
         lower_bound(val) -> 1st elem >= val    // first position where val would be inserted
         upper_bound(val) -> 1st elem > val     // last  position where val would be inserted
         equal_range(val) -> [l_b, u_b)         // the range of elements that have the value val

         if (l_b == u_b) || (l_b == end):
            => elem not in collection, insert here
         
         special cases?
            l_b==u_b    == begin          => insert elem at front, before *begin
            l_b==u_b    == end            => insert elem at back, @end
            l_b==u_b    != begin|end      => insert @l_b
- p.324: std::set / std::multiset
         - To remove all elements that have a certain value, use c.erase().
           Note that this member function has a different name than remove() provided for lists.
           It behaves differently in that it returns the number of removed elements.
           When called for sets, it returns only 0 or 1.
         - to remove only the ﬁrst element of multiple duplicates in a multiset use pos=c.find(val), c.erase(pos)
- p.325: "For multisets, all insert(), emplace(), and erase() operations
          preserve the relative order of equivalent elements.
          Since C++11, calling insert(val) or emplace(args...) guarantees
          that the new element is inserted at the end of the range of equivalent elements."
- p.328: specify sorting criterion at runtime (see also p.336)
- p.332: std::map / std::multimap
         - "For multimaps, the order of elements with equivalent keys is random but stable.
          Thus, insertions and erasures preserve the relative ordering of equivalent elements (guaranteed since C++11)."
         - Maps and multimaps sort their elements automatically, according to the element’s keys,
          and so have good performance when searching for elements that have a certain **key**
         - Searching for elements that have a certain **value** promotes bad performance
         - you may not change the key of an element directly
           => To modify the key of an element, you must
               - remove the element that has the old key
               - insert a new element that has the new key and the old value
- p.334: 2 ways of defining a sorting criterion
- p.335: std::map / std::multimap
         - c.find(val) searches for a **key** that equals val - cannot search for a certain **value**
         - to search for a certain **value**, use algo as find_if or explicit loop
- p.336: "The type of the comparison criteria must be the same,
          although the comparison criteria themselves may be different."
- p.339: change the key of an element in a map (see also p.347)
               function 'replace_key'
                  or
               m["new_key"] = m["old_key"];  // insert new element with value of old element
               m.erase("old_key");           // remove old element
- p.340: - "For multimaps, since C++11 it is guaranteed that insert(), emplace(), and erase()
            preserve the relative ordering of equivalent elements, and that inserted elements
            are placed at the end of existing equivalent values"
         - 3 ways to pass a value into a map or multimap + 1 (initlist) + 1 (operator[])
- p.343: remove element with a certain value from a map while iterating over it
- p.343: "For multimaps, all insert(), emplace(), and erase() operations
          preserve the relative order of equivalent elements.
          Since C++11, calling insert(val) or emplace(args...) guarantees that
          the new element is inserted at the end of the range of equivalent elements.
- p.344: map / multimap / unordered_map / unordered_multimap: operator[]
         - "The value of the new element is initialized by the default ctor of its type.
            Thus, to use this feature, you can’t use a value type that has no default ctor.
- p.345: inserting in map (1) is slower than (2)
            (1) m["pi"] = 3.14159;
            (2) m.insert({"pi", 3.14159})
- p.349: dictionary example (using multimap)
- p.350: searching an element with a certain **key** in a map    => logarithmic complexity
         searching an element with a certain **value** in a map  => linear complexity
- p.351: example for case-insensitive compare function messing up internals of map => use multimap for dictionary!
- p.357: "open hashing/closed addressing" vs. "closed hasing/open addressing"
         'chaining' approach (open hashing): a hash code is associated with a linked list
- p.358: for unordered containers (unordered_*):
            insertions, deletions, and element search is all O(1)
- p.359: disadvantages of unordered container vs. associative containers
- p.359: influence behaviour of hash table
            - min num of buckets
            - hash function
            - equivalence criterion
            - maximum load factor
            - force rehashing
         => rehashing is possible only after a call to insert(), rehash(), reserve(), clear()
         => It is guaranteed that erase() never invalidates iterators, references, and pointers to the elements.
            Thus, if you delete hundreds of elements, the bucket size will not change.
            But if you insert one element afterward, the bucket size might shrink.
         => for unordered_multi* that support equivalent keys:
            Rehashing and other operations that internally change the order of elements
            preserve the relative order of elements with equivalent keys.
- p.362: To specify the maximum load factor, you have to call a member function right after construction.
- p.362: max_load_factor, rehash
               if max_load_factor==0.7, and want to be prepared for 100 elements,
               then 
                  ceil(100/0.7) // == 143 // see also p.428
               is the size that does not cause rehashing as long as no more than 100 elements have been inserted
               => c.rehash(143)
               
               or simply
               => c.reserve(100)
- p.363: provide own hash function
- p.364: hash function of boost
- p.366: provide own equivalence criterion (global operator==) for custom class to use it inside unordered_[set|map]
- p.367: An unordered container that is instantiated with a nondefault equivalence predicate
         usually needs a nondefault hash function as well.
- p.370: change the key of an element in a unordered_map (see p.339)
- p.370: unordered_*
         - erasing functions do not invalidate iterators and references to other elements
         - however, the insert() and emplace() members may invalidate all iterators when rehashing happens,
           whereas references to elements always remain valid
         - rehashing happens when, because of an insertion,
           the number of resulting elements is equal to or exceeds the bucket count times the maximum load factor
           (i.e., when the guarantee given by the maximum load factor would be broken)
         - the insert() and emplace() members do not affect the validity of references to container elements
- p.374: hash table bucket interface
- p.376: "As long as you only insert, erase, and ﬁnd elements with a speciﬁc value,
          unordered containers provide the best running-time behavior
          because all these operations have amortized constant complexity.
          However, you can’t make any assumption about the order of the elements."
- p.377: provide own hash function and equivalence criterion
- p.379: equivalence function and hash function
         "The equivalence function does not necessarily have to evaluate the same values as the hash function.
          However, as written, it should be guaranteed that values that are equal according the equivalence criterion
          yield the same hash value (which indirectly is the case here assuming that customer numbers are unique)"
- p.380: lambda as hash function and equivalence criterion
         unordered_set<Customer, decltype(hash), decltype(eq)> custset(10, hash, eq);
- p.380: using the bucket interface
- p.380: for unordered_multimap
         "Rehashing preserves the relative ordering of equivalent elements.
          However, the order of equivalent elements might not match the order of their insertion."
- p.385: open closed principle: open to extension, closed to modification
         3 approaches to making containers "STL-able":
            - invasive
            - non-invasive
            - wrapper
- p.388: implementing reference semantics for STL containers
         - using shared_ptr
         - using reference_wrapper
- p.392: when to use which container (table on p.393)
         - "Like all node-based containers, a list doesn’t invalidate iterators that refer to elements,
            as long as those elements are part of the container"
         - "Vectors invalidate all their iterators, pointers, and references whenever they exceed their capacity
            and part of their iterators, pointers, and references on insertions and deletions"
         - "Deques invalidate iterators, pointers, and references when they change their size"
- p.394: how to sort objects according to two different sorting criteria?
         => probably use two sets or two maps that share the same objects with different sorting criteria
- p.394: sort all strings read from stdin and print them without duplicates
- p.397: STL container members in detail
         container::iterator
         container::local_iterator  // bucket iterator of unordered containers
         ...
- p.404  bool operator==(const container& c1, const container& c2), and
         bool operator!=(const container& c1, const container& c2)
         => O(n) in general, for unordered containers O(n^2) in worst case

         bool operator<(const container& c1, const container& c2), and other comparisons <=, >, >=
         => O(n)

         for associative and unordered containers, O(log(n)) instead of O(n)
            container::count        => O(log(n))
            container::find         => O(log(n))
            container::lower_bound  => O(log(n)) // p.405
            container::upper_bound  => O(log(n))
            container::equal_range  => O(log(n)) // p.406

         swap, container::swap  => O(1) in general, O(n) for arrays // p.407
            - The references, pointers, and iterators referring to elements swap their containers,
              because they still refer to the same swapped elements afterward
            - Arrays can’t internally just swap pointers. Thus, swap() has linear complexity,
              and iterators and references refer to the same container but different elements afterward.
- p.410: iterator category of each STL container
- p.412: container::insert(val)
            - For all containers, references to existing elements remain valid.
            - For associative containers, all iterators to existing elements remain valid.
            - For unordered containers, iterators to existing elements remain valid
              if no rehashing is forced (if the number of resulting elements is
              equal to or greater than the bucket count times the maximum load factor)
         container::emplace(args)
            - For all containers, references to existing elements remain valid.
            - For associative containers, all iterators to existing elements remain valid.
            - For unordered containers, iterators to existing elements remain valid
              if no rehashing is forced (if the number of resulting elements is
              equal to or greater than the bucket count times the maximum load factor)
         container::insert(pos,val)
            - For associative and unordered containers, the position is used only as a hint
              pointing to where the insert should start to search. If value is inserted right at pos,
              the function has O(1); otherwise, it has O(log(n)).
            - For vectors, this operation invalidates iterators and references to other elements
              if reallocation happens (the new number of elements exceeds the previous capacity).
            - For deques, this operation invalidates iterators and references to other elements.
         container::emplace(pos,val)
            - For vectors, this operation invalidates iterators and references to other elements
              if reallocation happens (the new number of elements exceeds the previous capacity).
            - For deques, this operation invalidates iterators and references to other elements.
         container::emplace_hint(pos,args)
            - If the new element is inserted at pos, the function has O(1); otherwise, it has O(log(n)).
         container::push_front
            - For deques, this operation invalidates iterators to other elements.
              References to other elements remain valid.
         emplace::push_front
            - For deques, this operation invalidates iterators to other elements.
              References to other elements remain valid.
         container::push_back
            - For vectors, this operation invalidates iterators and references to other elements
              if reallocation happens (the new number of elements exceeds the previous capacity).
            - For deques, this operation invalidates iterators to other elements.
              References to other elements remain valid.
         container::emplace_back
            - For vectors, this operation invalidates iterators and references to other elements
              if reallocation happens (the new number of elements exceeds the previous capacity).
            - For deques, this operation invalidates iterators to other elements.
              References to other elements remain valid.
         container::insert(initlist)
            - For all containers, references to existing elements remain valid.
            - For associative containers, all iterators to existing elements remain valid.
            - For unordered containers, iterators to existing elements remain valid
              if no rehashing is forced (if the number of resulting elements is
              equal to or greater than the bucket count times the maximum load factor)
         container::insert(pos,initlist)
            - For vectors, this operation invalidates iterators and references to other elements
              if reallocation happens (the new number of elements exceeds the previous capacity).
            - For deques, this operation invalidates iterators and references to other elements.
         container::insert(pos,num,val)
            - For vectors, this operation invalidates iterators and references to other elements
              if reallocation happens (the new number of elements exceeds the previous capacity).
            - For deques, this operation invalidates iterators and references to other elements.
         container::insert(beg,end)
            - For all containers, references to existing elements remain valid.
            - For associative containers, all iterators to existing elements remain valid.
            - For unordered containers, iterators to existing elements remain valid
              if no rehashing is forced (if the number of resulting elements is
              equal to or greater than the bucket count times the maximum load factor).
         container::insert(pos,beg,end)
            - For vectors, this operation invalidates iterators and references to other elements
              if reallocation happens (the new number of elements exceeds the previous capacity).
            - For vectors and deques, this operation might invalidate iterators and references to other elements.
            - For lists, the function either succeeds or has no effect.
         container::erase(val)   // p.418
            - The function does not invalidate iterators and references to other elements.
            - For (forward) lists, remove() provides the same functionality.
         container::erase(pos)
            - For vectors and deques, this operation might invalidate iterators and references to other elements.
            - For all other containers, iterators and references to other elements remain valid.
         container::erase(beg,end)
            - For vectors and deques, this operation might invalidate iterators and references to other elements.
            - For all other containers, iterators and references to other elements remain valid.
         container::pop_front
            - Iterators and references to other elements remain valid.
            == container.erase(container.begin()) or
            == container.erase_after(container.before_begin())
         container::pop_back
            - Iterators and references to other elements remain valid.
            == container.erase(prev(container.end()))
         container::clear
            - Invalidates all iterators and references to elements of the container.
            - For vectors, deques, and strings, it even invalidates any past-the-end-iterator,
              which was returned by end() or cend()
         container::resize(num)    // p.420
            - how it operates:

               if size() == num:
                  return
               elif num > size():
                  create additional elems and append to end (default ctor is called (or copy ctor?))
               elif num < size():
                  remove elems at the end to get the new size (dtor is called for removed elems)

            - For vectors and deques, this operation might invalidate iterators and references to other elements.
            - For all other containers, iterators and references to other elements remain valid.
            - For vectors and deques, these functions either succeed or have no effect,
              provided that the constructor or the assignment operator of the elements doesn’t throw.
            - For lists and forward lists, the functions either succeed or have no effect.
         forwardlist::erase_after   // p.425
            - Iterators and references to other elements remain valid.
         forwardlist::splice_after(pos,src)
            - Pointers, Iterators and references to members of src remain valid.
              Thus, they belong to this afterward.

         container::max_load_factor()    // p.427
            - default is 1.0, which usually should be modified (see p.362)
         container::reserve(num)
            - For vectors, this call can only increase the capacity.
              Thus, it has no effect if num is <= to the actual capacity.
            - for unordered containers
               - This call is equivalent to rehash(ceil(num/max_load_factor))
               - This operation invalidates iterators, changes ordering between elements,
                 and changes the buckets the elements appear in.
                 The operation does not invalidate pointers or references to elements.
            - This operation might invalidate iterators and (for vectors and strings) references and pointers to elements.
              However, it is guaranteed that no reallocation takes place during insertions
              that happen after a call to reserve() until the time when an insertion would make the size greater than num.
              Thus, reserve() can increase speed and help to keep references, pointers, and iterators valid
         container::rehash(num_buckets)
            - This operation invalidates iterators, changes ordering between elements,
              and changes the buckets the elements appear in.
              The operation does not invalidate pointers or references to elements.
         container::max_load_factor(load_factor)
            - This operation might cause a rehashing, which invalidates iterators,
              changes ordering between elements, and changes the buckets the elements appear in.
              The operation does not invalidate pointers or references to elements.
         container::begin(bucket_idx)     // p.429
         container::container(num,val)    // p.432 -- e.g.: std::string(40,'~')
- p.434: iterator categories, output iterators
         - can't use an output iterator to iterate twice over the same range
- p.437: forward iterator example: process adjacent duplicates
- p.438: random-access iterator provide ops for iterator arithmetic (akin to ptr arithmetic)
- p.440: inc/dec temporary iterators of arrayvector/string not portable, e.g.: ++coll.begin()
         => use std::next(), std::prev()
- p.441: auxiliary iterator functions: advance(), next(), prev(), distance()
- p.444: note: checking for 'pos != coll.end()' every iter and then 'nextPos != coll.end()' in
            'while (pos != coll.end() && nextPos != coll.end())'
         doesn't make sense => only need to check for 'pos != coll.end()' once at very beginning,
         then every iter check only 'nextPos != coll.end()'
- p.446: note on using std::distance()
         "If you don’t know which iterator position comes ﬁrst,
          you have to process the distance between both iterators to the beginning of the container
          and process the difference of these distances."
- p.448: all containers (except forward_list and unordered_*) and strings provide reverse iterators
- p.449: reverse iterator
         rbegin():   returns pos of 1st elem of a reverse iteration, i.e.           pos of last elem
         rend():     returns pos after the last elem of a reverse iteration, i.e.   pos before 1st elem

         conversion between normal and reverse iterators possible, but BEWARE:
            the logical position of an iterator is moved during the conversion normal->reverse
         for reverse iterator: physical pos != logical pos
         => advantage: when converting a range speciﬁed by 2 iterators, all elems remain valid

         to convert reverse->normal: rpos.base()
- p.454: for insert iterators
            *pos = val;
         is translated into
            pos.op(val), where op \in {push_back(), push_front(), insert()} of container
         TODO: write insert iterator for file, memory block, ...
- p.456: !!! need reserve before copy, even for back_inserter (inserting into same collection which is iterated over) !!!
            "Note that you must not forget to reserve enough space before calling copy().
             The reason is that the back inserter inserts elements,
             which might invalidate all other iterators referring to the same vector.
             Thus, if not enough space is reserved, the algorithm invalidates the passed source iterators while running."
            => ok, makes sense...
- p.462: end-of-stream iterator  // istream_iterator<T>()
- p.463: two istream_iterator are equal if
         - both are end-of-stream iterators and thus can no longer read, or
         - both can read and use the same stream
- p.465: using 'istream_iterator<string>' and 'ostream_iterator<string>' to write every 3rd string from cin to cout
- p.471: writing own iterator // insert iterator for associative and unordered containers
- p.475: functor
- p.478: generate_n, generate
- p.479: functors are passed by value rather than by reference by default
         =>    advantage: can pass constant and temporary expressions
         => disadvantage: modified state of functor only visible inside algo, can't access/process functor's final state
         
         3 ways to get result from functor passed to algos:
         - keep state externally, let functor refer to it
         - pass functor by ref
         - use return value of for_each algo
- p.484: functor as predicate
         - should not pass a functor for which the behavior depends on how often it is copied or called
         - a predicate should always be stateless! so rather a lambda than a functor!
         - always declare the function call operator of predicates/functors as being a constant member function.
- p.487: ascending order: element < nextElement
- p.488: std::bind, std::placeholder::_1
- p.489: f = bind(plus<int>(), _1, 10) // f(param1) == param1+10
- p.490: search substring case insensitive, using bind (version using lambda on p.503)
- p.499: lambdas vs. binders
- p.507: <algo> vs <algo>_if, <algo> vs. <algo>_copy
            find()         find elem with certain val
            find_if()      find elem that matches the criterion passed as function/functor/lambda
            <algo>_copy    elems are not only manipulated but also copied into a destination range
- p.509: string-function vs. STL-algo
- p.511: removing algos
         - these algorithms remove elements logically only by overwriting them
           with the following elements that were not removed. Thus, removing algorithms
           do not change the number of elements in the ranges on which they operate.
           Instead, they return the position of the new “end” of the range. (see also p.575)
- p.512: nth_element() stops sorting when the nth elem of a sequence is correct according to the sorting criterion
- p.514: make_heap, sort_heap
         "a heap can be considered as a binary tree implemented as sequential collection"
- p.514: nth_element()
         - the nth_element() algos are provided if you need only the nth sorted element
           or the set of the n highest or n lowest elements (not sorted)
         - nth_element() vs. partition() vs. stable_partition()
- p.516: sorted-range algorithms
- p.522: MeanValue functor with implicit conversion
- p.525: count_if() + lambda to count the number of even/odd values in a collection
- p.526: min_element, max_element, minmax_element
         => if the range is empty, the algorithms return 'beg' or a 'pair<beg,beg>'
- p.528: if range is sorted, use lower_bound(),upper_bound(),equal_range(),binary_search(),
         instead of find(),find_if(),find_if_not()
- p.528: for associate and unordered containers, use member function find()
- p.528: ﬁnd a subrange starting with the 1st element with value 4 and ending after the second 4, if any
- p.534: find a sequence as subrange of another sequence (all occurrences of subrange)
- p.535: find a sequence "even, odd, even"
- p.548: lexicographical_compare (collection of collections)
- p.557: different forms of copy, and when to use which
- p.575: removing algos
         - can't use these algos for
            - associative or unordered containers  (they provide a similar member function erase())
            - lists                                (they provide an equivalent member function remove())
- p.578: removing consecutive duplicates: unique()
         - can't use these algos for
            - associative or unordered containers
            - lists                                (they provide an equivalent member function unique())
         - the predicate is not used to compare an element with its predecessor;
           the element is compared with the previous element that was not removed
- p.582: copy stdin to stdout while compressing spaces
- p.584: rotate left/right
- p.593: stable_partition(), unlinke partition(), preserves the relative order
- p.594: copy all elements partitioned accordingly into even and odd elements (with one call)
- p.599: stable_sort() preserves relative order of elements
- p.604: heap properties
            1. the 1st element is always (one of) the largest
            2. you can add or remove an element in logarithmic time
- p.606: using heap algos
- p.607: figure of heap as binary tree
- p.608: sorted-range algorithms: undefined behaviour (UB) for calling these algos with unsorted sequences!
- p.609: includes(): search for a subset
- p.611: lower_bound, upper_bound (or equal_range to obtain both)
            lower_bound(b,e,val): 1st elem >= val; return end if no such value
               the **first** position where an elem with value val could get inserted
               without breaking the sorting of the range
            upper_bound(b,e,val): 1st elem >  val; return end if no such value
               the **last** position where an elem with value val could get inserted
               without breaking the sorting of the range
            
            => val could be inserted anywhere in [l_b,u_b] // see example p.612

            as I understand, insert val @pos i means:
               shift all elems [i,end) one to right, insert val at i

                  1   2   3   4   5               1   2   3   4   5   6
             ~~~+~~~+~~~+~~~+~~~+~~~+~~~     ~~~+~~~+~~~+~~~+~~~+~~~+~~~+~~~
             ...| 4 | 4 | 5 | 7 | 7 |...     ...|   | 4 | 4 | 5 | 7 | 7 |...
             ~~~+~~~+~~~+~~~+~~~+~~~+~~~     ~~~+~~~+~~~+~~~+~~~+~~~+~~~+~~~
                  ^                               ^
                  |                               |
                  ` l_b(4) == 1                    put 4 here

               1   2   3   4   5               1   2   3   4   5   6
          ~~~+~~~+~~~+~~~+~~~+~~~+~~~     ~~~+~~~+~~~+~~~+~~~+~~~+~~~+~~~
          ...| 5 | 7 | 7 | 8 |11 |...     ...| 5 |   | 7 | 7 | 8 |11 |...
          ~~~+~~~+~~~+~~~+~~~+~~~+~~~     ~~~+~~~+~~~+~~~+~~~+~~~+~~~+~~~
                   ^                               ^
                   |                               |
                   ` l_b(6) == 2                    put 6 here


            v: 1 1 2 4 4 5 7 7 8 11 15 17 19
                     |   |
         l_b(b,e,4) .´   `. u_b(b,e,4)
                  
            v: 1 1 2 4 4 5 7 7 8 11 15 17 19
                         | |
             l_b(b,e,5) .´ `. u_b(b,e,5)

            v: 1 1 2 4 4 5 7 7 8 11 15 17 19
                          |
              l_b(b,e,6) .^. u_b(b,e,6)
- p.616: set_union
            - "The number of occurrences of equal elements in the destination range
               is the maximum of the number of their occurrences in both source ranges."
            - to obtain all elements of both source ranges without removing elements that are in both,
              use merge() (p.614)
- p.617: set_intersection
            - "The number of occurrences of equal elements in the destination range
                is the minimum number of their occurrences in both source ranges."
- p.616-617: set_union (OR), set_intersection (AND)
               to really get union/intersection, have to call unique afterwards ?!?
- p.618: set_difference
            - "The number of occurrences of equal elements in the destination range
               is the difference between the number of their occurrences in the ﬁrst source range
               less the number of occurrences in the second source range.
               If there are more occurrences in the second source range,
               the number of occurrences in the destination range is zero."
- p.618: set_symmetric_difference
            - "The number of occurrences of equal elements in the destination range
               is the difference between the number of their occurrences in the source ranges."
- p.620: example of all merging algorithms (merge, union, intersection, difference, symmetric difference)
- p.623: accumulate, for: sum, product
            initValue +  a1 +  a2 +  a3 +  ...
            initValue op a1 op a2 op a3 op ...
- p.623: inner_product
            initValue +   (a1 *   b1) +   (a2 *   b2) +   (a3 *   b3) +   ...
            initValue op1 (a1 op2 b1) op1 (a2 op2 b2) op1 (a3 op2 b3) op1 ...
- p.627: converting relative values to absolute values
            partial_sum          (complement to adjacent_difference)
               {a1 a2 a3 ...} => {a1, a1 +  a2, a1 +  a2 +  a3, ...}
               {a1 a2 a3 ...} => {a1, a1 op a2, a1 op a2 op a3, ...}
- p.630: converting absolute values to relative values
            adjacent_difference  (complement to partial_sum)
               {a1 a2 a3 a4 ...} => {a1, a2 -  a1, a3 -  a2, a4 -  a3, ...}
               {a1 a2 a3 a4 ...} => {a1, a2 op a1, a3 op a2, a4 op a3, ...}
- p.632: stack: underlying container is deque<T> by default
- p.633: stack (LIFO)   -- push(), pop(), top()
            to add elem on stack:            s.push(e);
            to only remove next elem:        s.pop();
            to process & remove next elem:   e=s.top(); s.pop();
            note:
               - pop() removes next elem, but doesn't return it
               - top() returns next elem, but doesn't remove it
- p.639: queue: underlying container is deque<T> by default
- p.639: queue (FIFO)   -- push(), pop(), front(), back()
            to add elem on Q:                q.push(e);
            to only remove next elem:        q.pop();
            to process & remove next elem:   e=q.front(); q.pop();
            note:
               - pop() removes next elem, but doesn't return it
               - front() and back() return the elem, but dont't remove it
               - front() returns the next elem in the queue (the elem that was inserted ﬁrst)
               - back()  returns the last elem in the queue (the elem that was inserted last)
- p.642: priority_queue -- push(), pop(), top()
         priority queue with reverse sorting:
            std::priority_queue<float, std::vector<float>, std::greater<float>> pq;
- p.643: priority_queue
            - pop() removes next elem, but doesn't return it
            - top() returns next elem, but doesn't remove it
- p.644: priority_queue, uses STL heap algos
- p.648: top()/front() returns the next elem of container adapter (stack/queue/priority_queue)
         - top()     for stack:            next elem is the one that was inserted last
         - front()   for queue:            next elem is the one that was inserted first
         - top()     for priority_queue:   next elem is the one with max value
- p.648: pop() removes the next elem from container adapter (stack/queue/priority_queue)
         - for stack:            next elem is the one that was inserted last
         - for queue:            next elem is the one that was inserted first
         - for priority_queue:   next elem is the one with max value
- p.648: back() returns the last elem of container adapter (queue)
         - for queue:            last elem is the one that was inserted last
- p.652: bitset, print numbers in binary representation, binaryNum2string, string2binaryNum
- p.655: strings / C-strings
         - the original type of a literal such as "hello" is 'const char[6]',
           but this type automatically converts (decays) to 'const char*'
- p.656: "Strings are indirectly required to provide an end-of-string character (’\0’ for string)
          because for a string s, s[s.length()] is always valid
          and s.data() returns the characters including a trailing end-of-string character."
          note: see also p.662 for 's[s.length()] // yields ’\0’', p.671
- p.657: string, substr, split filename into basename and extension
- p.658: use 'std::string::size_type' (or auto) and NOT int as return type
         for s.find, to compare with std::string::npos (see also p.680)
- p.660: size and length both return the same: length of string
         (same as strlen() for C-strings; strlen does NOT include '\0')
- p.661: line by line (using getline), separate all words in one line
- p.661: getline()
         The function getline() is a special function to read input from streams into a string.
         It reads every character up to the next end-of-line, which by default is the newline character.
         The line delimiter itself is extracted but not appended.
- p.664: basic_string<>
         basic_string<char> == string
- p.669: string to C-string
         - copy contents of string into character array
         - the return value of c_str() and data() is valid only
           until the next call of a nonconstant member function for the same string
         - call function for char-array and the number of chars: f(s.data(),s.length())
- p.670: string: size, length, capacity, reserve
         - size == length: same as strlen() for C-strings; strlen does NOT include '\0'
         with regards to capacity:
         - reallocation invalidates all references, pointers, and iterators that refer to characters of the string
- p.671: "The standard, however, speciﬁes that capacity may shrink only
          because of a call of reserve() or shrink_to_fit().
          Thus, it is guaranteed that references, pointers, and iterators remain valid
          even when characters are deleted or modiﬁed, provided that they refer to characters
          having a position that is before the manipulated characters."
- p.672: string invalidation of refs, prts, iters
         - References, pointers, and iterators that refer to characters of a string
           may be invalidated by the following operations:
           - if the value is swapped with swap()
           - if a new value is read by operator>>() or getline()
           - if any nonconstant member function is called,
             except operator[], at(), begin(), end(), rbegin(), and rend()
- p.673: comparison of strings
         - strings are compared lexicographically (as they would be listed in a dictionary)
         - s.compare() example:
               std::string s("abcd");
               s.compare(0,2,s,2,2)    // returns a value <0 ("ab" is less than "cd")
- p.674: make string empty -- to remove all chars in a string:
         - s = "";
         - s.clear();
         - s.erase();
- p.675: careful when using s.insert()
- p.676: s.replace() and s.erase() examples
- p.676: s.resize():
            If the new size that is passed as an argument is less than the current number of characters,
            characters are removed from the end.
            If the new size is greater than the current number of characters,
            characters are appended at the end.
- p.676: s.substr()
- p.677: getline (no std:: needed, because of ADL)
- p.682: numeric conversions for strings: stoi, stol, stoul, stof, stod...
- p.683: hex-number-string to integer
- p.684: string iterators are invalidated when reallocation occurs or
         when certain changes are made to the values to which they refer. (see also p.672)
- p.686: uppercase all chars in a string, search case-insensitive in string
- p.687: search for regex-pattern in string
- p.689: read all chars from stdin into a string while compressing whitespaces (see also p.582)
- p.690: special traits class for strings so that they operate in a case-insensitive way
- p.690: differences between strings & vectors
- p.694: std::string::npos
- p.698: string::reserve()
         - the capacity is never reduced below the current number of characters
         - Might invalidate references, pointers, and iterators to characters.
           However, it is guaranteed that no reallocation takes place during insertions
           that happen after a call to reserve()
           until the time when an insertion would make the size greater than num.
           Thus, reserve() can increase speed and help to keep references, pointers, and iterators valid.
- p.718: regex: ﬁnd XML/HTML-tagged value (using default syntax)
- p.719: raw string
- p.720: "(.|\n)*" stands for any number of any character --  ".": any char except '\n' (see also p.738)
            regex_search(data, pattern) ==
            regex_search(data, "(.|\n)*" + pattern + "(.|\n)*")
- p.724: regex in loop, continue after each match
- p.724: regex to find inner XML tags (TODO: error here ?!? => '>' instead of '<')
- p.730: replace regex match
- p.733: regex to find LaTeX index entries
- p.736: bracket: '[' or ']', paren: '(' or ')', brace: '{' or '}'
- p.738: modified ECMAScript grammer
- p.739: BRE: basic regular expression, ERE: extended regular expression
- p.740: regex grammer differences (see egrep)
- p.744: istream == basic_istream<char>, ostream == basic_ostream<char> (see also p.751)
- p.746: endl: '\n' + flush()
- p.748: basic_streambuf<> performs the actual reading/writing
- p.749: - basic_streambuf<>: for access to some external representation, classes are derived from basic_streambuf<>
         - classes derived from basic_ios (the streams)  handle only formatting of data (they delegate it)
         - reading & writing of chars is done by stream buffers (see also p.826)
- p.750: stream buffer
            basic_streambuf<char> == streambuf
- p.751: global stream objects
            basic_istream<char> == istream   // cin
            basic_ostream<char> == ostream   // cout
         - by default, C++ streams are sync'ed with C streams
           => turn if off with 'std::ios::sync_with_stdio(false)'
- p.752: synchronization between C++ streams and C streams: sync_with_stdio(false)
- p.752: including <iostream> can be expensive! (see also p.844)
         - <iomanip> : manipulators
         - <fstream> : file streams
         - <sstream> : string streams
- p.753: The output operators (<<) for streams are deﬁned to send their 2nd argument
         to the corresponding stream, i.e.:
            std::ostream& operator<<(std::ostream& os, const T& obj)
            {
               os << ...
               return os;
            }
- p.754: operator<< is evaluated from left to right
- p.754: the input operators (>>) for streams are deﬁned to store the value read in their 2nd argument,
         i.e. (see also p.812):
         std::istream& operator>>(std::istream& is, T& obj)
         {
            ...
            is >> ...
            obj = ...
            return is;
         }
- p.755: when using cin/operator>>, leading whitespace is skipped by default (change that using skipws)
         (see also p.761)
- p.755: input/operator>>
         - When reading numeric values (int x; cin >> x;), the input must start with at least one digit,
           otherwise, the numeric value will be set to 0 and the failbit is set.
         - When a char is being read with operator>>, leading whitespace is skipped by default. (see p.767)
           To read any character, including whitespace, 
           you can either clear the ﬂag skipws (see p.789) or use the member function get().
- p.756: read C-string/char array with cin (better use getline and string, or p.770: stream member functions)
- p.758: The constant goodbit is deﬁned to have the value 0.
         Thus, having goodbit set means that all other bits are cleared.
         The name goodbit may be somewhat confusing because it doesn’t mean that any bit is set.
- p.758: eofbit & failbit are set together... (see also p.761, p.764)
- p.759: The stream buffers don’t have state ﬂags.
         One stream buffer can be shared by multiple stream objects,
         so the ﬂags represent only the state of the stream as found in the last operation.
         Even this is the case only if goodbit was set prior to this operation.
         Otherwise, the ﬂags may have been set by an earlier operation.
- p.759: rdstate()
- p.759: set bit / clear bit in flags
            flags |= bit_to_set;
            flags &= ~bit_to_clear;
- p.760: If failbit is set, each following stream operation is a no-op until failbit is cleared explicitly.
- p.761: loop that reads and processes objects: while(cin >> obj) { cout << obj << endl; }
         - as 'cin >> obj' returns 'cin'
- p.761: if whitespace should not be skipped, use put() or get() or istreambuf_iterator
- p.762: streams and exceptions
         - for streams, one can deﬁne, for every state ﬂag, whether setting that ﬂag will trigger an exceptiion
         - by default, streams don't throw any exception
         - to explicitly set that no exception should be thrown (p.763):
            'strm.exceptions(std::ios::goodbit);' or
            'strm.exceptions(0);'
- p.764: distinguish EOF from input error
- p.767: use stream's member functions to read unformatted data, instead of using operator<< / operator>>,
         of stream buffer iterators (see p.828)
- p.768: 'int istream::get()' <–> C's getchar() or getc()
         - note that istream::get() returns int, not char! (to distinguish EOF from chars (see also p.855)),
           so should probably be used as follows:

               std::istringstream iss(...);   #|#   std::istringstream iss(...);
               ...                            #|#   ...
               char c;                        #|#   char c;
               int ret = iss.get();           #|#   iss.get(c);
               if (ret == EOF) {              #|#   if (iss.eof()) {
                  handle_eof();               #|#      handle_eof();
               } else {                       #|#   } else {
                  c = (char)ret;              #|#      ...
                  ...                         #|#   }
               }                              #|# 

- p.770: discard the rest of the line: cin.ignore(numeric_limits<std::streamsize>::max(),’\n’);
- p.771: might be better to use stream buffer directly, or istreambuf_iterator<>/ostreambuf_iterator<>:
         "It is often better to use the stream buffer directly instead of using istream member functions.
          Stream buffers provide member functions that read single characters or character sequences efﬁciently,
          without overhead due to the construction of sentry objects."
          (see also p.828)
- p.772: write all read chars (see also p.831(uses [i|o]streambuf_iterator<char>), p.846(uses stream buffer))
- p.774: setw, setfill
- p.775: setw, right, fixed, ...
- p.776: impl of manipulators, impl of std::endl
- p.777: user-defined manipulators => 'std::cin >> ignoreLine;'
- p.779: access format flags, e.g. flags(), copyfmt(stream):
            
            - auto old_flags = cout.flags();
              ...
              cout.flags(old_flags);
            
            - std::ostringstream s;
              s.copyfmt(cout);
              ...
              cout.copyfmt(s);

- p.780: save current format flags of cout, do sth, restore saved flags
- p.782: stream.width(), stream.fill(), ...
         - width(fw) defines minimum field width (default value is 0 => field may have any length)
         - cout.width(fw) -- only applies to the next formatted field written
         - can't specify max field with, have to program it
- p.783: manipulator std::setw() -- only applies to the next formatted field written
         - std::setw() <–> stream.width()
         - cout << setw(4) << 1 << " ";
- p.783: using std::setw() when reading from cin into C-string
- p.785: numeric base flags
         - A number starting with 0x or 0X is read as a hexadecimal number.
           A number starting with 0 is read as an octal number.
           In all other cases, the number is read as a decimal value.
- p.785: manipulators std::oct, std::dec, std::hex
- p.785: showbase (e.g.: cout << std::hex << std::showbase << some_num;)
- p.788: printing float: std::fixed, std::setprecision()
- p.791: file stream classes
            basic_ifstream<char> == ifstream
            basic_ofstream<char> == ofstream
            basic_fstream<char>  == fstream
            basic_filebuf<char>  == filebuf
- p.792: 
         - Fle stream classes automatically manage ﬁles.
           The ﬁles are automatically opened at construction time and closed at destruction time.
         - "For streams that are both read and written,
            it is important to note that it is not possible to switch arbitrarily between reading and writing.
            Once you start to read or write a ﬁle you have to perform a seek operation,
            potentially to the current position, to switch from reading to writing or vice versa.
            The only exception to this rule is if you have read until end-of-ﬁle.
            In this case, you can continue writing characters immediately."
- p.794: write chars to file, read that file and output to cout
            std::ofstream file(filename);
               or
            std::ofstream file;
            file.open(filename) // see p.798
- p.797: compare flags with C's flags
            set std::ios::binary <–> "...b"
            set std::ios::ate    <–> seek to end of file immediately after opening

         Whether a ﬁle is opened for reading and/or for writing is independent of the
         corresponding stream object’s class. The class determines only the default open mode
         if no second argument is used. This means that
         ﬁles used only by the class ifstream or the class ofstream can be opened for reading AND writing.
- p.798: open/close explicitely multiple filenames with a single ifstream (cat)
- p.799: tellg, seekg, tellp, seekp -- g:get, p:put
           |______|      |______|
           istream       ostream
         
         e.g.: iss.seekg(-1,std::ios::end); // seek to last char of istream
- p.800: - if using tellg/tellp, use std::ios::pos_type, not int or sth !
         - for relative position: std::ios::beg, std::ios::cur, std::ios::end
- p.801: - operate directly on the stream buffer, which can’t manipulate the state of the stream
         - for the standard streams, the same position is maintained for reading and writing
           in the same stream buffer
         - init file stream with file descriptor
- p.804: stringstream
            basic_istringstream<char> == istringstream // to read from strings
            basic_ostringstream<char> == ostringstream // to write to strings
            basic_stringstream<char>  == stringstream
- p.804: stringstream, stringstream::str()
- p.805: remove the current contents from the stream: strm.str("");
- p.806: - std::ios::ate to append chars written to string stream to existing string
         - use temporary istringstream to parse/split a single string
- p.810: implementing output operator for own type (consider setw/left/...)
            'stream << object' can be interpreted in 2 ways
               stream.operator<<(object)
               operator<<(stream,object)
- p.811: global operator<< for own type, taking into account previously set field width
- p.813: impl for global operator>> with transaction semantics
         (succeeds completely in all substeps, or not at all)
- p.815: friend function vs. delegating for streams (if impl of I/O requires access to private members)
- p.816: user-defined format flags: std::ios_base::xalloc(), strm.iword(), strm.pword()
- p.817: copyfmt() copies all format information, including the arrays accessed with iword() and pword()
- p.819: tie streams to one another (loose coupling using tie())
         - cin is tied to cout by default
- p.820: tie streams to one another (tight coupling using rdbuf(), use of a common stream buffer)
- p.821: format applies to the stream object (e.g.: std::ostream), not to the stream buffer (e.g.: cout.rdbuf())
         - disadvantage of this approach:
             construction and destruction of a stream object involve more overhead
             than just setting and restoring some format ﬂags
- p.822: redirecting cout to a file
         - redirect stream by setting a stream buffer
         - !!always save old buffer and restore it later!!
- p.824: create file stream buffer & install in ostream and istream
         - basic_filebuf<char> == filebuf
- p.825: write to (with ostream) and read from (with istream) same underlying filebuf
         - stream buffer member function called by seekg()/seekp() can distinguish between read & write pos
           (is seekg()/seekp() calling pubseekpos() or pubseekoff() ??? (p.828))
         - only standard stream buffers maintain 1 pos for reading & writing
- p.826: - reading & writing not done by streams directly but delegated to stream buffers (see also p.749)
         - for stream buffer interface, see p.820
- p.828: pubseekpos() / pubseekoff()
         - manipulate current pos used for reading and/or writing with stream buffer
         - invalid stream pos: pos_type(off_type(-1))
         - current pos of stream: sbuf.pubseekoff(0,std::ios::cur)
         - can use stream buffer iterators for unformatted I/O
- p.828: stream buffer iterators
            istreambuf_iterator<> //  input stream buffer iterators
            ostreambuf_iterator<> // output stream buffer iterators
         are used to read/write individual chars from/to objects of type basic_streambuf<>
- p.828: 'stream buffer iterators' are specials forms of 'stream iterators'
- p.831: can pass stream buffer iterators to algos to process all chars read from an input stream
- p.832: streambuf -- pbase, pptr, epptr, overflow, xsputn -- user-defined stream buffers
            interface for writing to   stream buffers (write buffer): pbase(), pptr(), epptr()   (p.832)
            interface for reading from stream buffers (read buffer):  eback(), gptr(), egprt()   (p.840)
- p.832: output stream buffers
            pbase()  beginning of output buffer
            pptr()   current write pos
            epptr()  end of output buffer
            - chars in [pbase,pptr) already written, but not yet transported/flushed to output channel
- p.835: stream buffer for file descriptor
- p.833: stream buffer with no buffer
- p.836: defining both own streambuf and own ostream
- p.837: own streambuf that buffers, setp, pptr, pbump, overflow flush buffer
- p.838: setting up write buffer with setp() determines when overflow() is called
- p.839: input stream buffers
            eback()  beginning of input buffer
            gptr()   current read pos
            egptr()  end of input buffer
            - chars in [gptr,egptr) already transported to memory of program, but not yet processed by program
- p.843: own input streambuf
- p.845: std::ios::sync_with_stdio()
- p.845: buffered I/O for efficiency
- p.846: - importance of buffering for input
         - use stream buffers directly
- p.849: i18n == internationalization
- p.851: character encoding:
            multibyte representation: used to store data outside of programs  (varying  num of bytes per char, use shift states)
            wide-character representation: used inside programs               (constant num of bytes per char)
- p.854: char_traits member functions
- p.855: implementation of 'struct char_traits<char>', using C-functions in string.h (see also p.689)
- p.857: locale: a collection of parameters and functions used to support national or cultural conventions
         check 'echo $LANG' => 'en_US.UTF-8'
- p.864: C and C++ locale mechanism only loosely coupled -- how to install a facet in a locale
- p.865: TODO: why is this an error ?!? reference should still be valid (see p.880)
- p.866: a C++ locale is an immutable container for facets
- p.869: (two) requirements of a class to be used as a facet + two impl guidelines
- p.869: impl guideline 2.:
         non-virtual public member functions delegate to protected virtual functions
         note: protected members and methods in base class are accessible in derived classes,
               so might make sense if someone will derive from base class for easy/efficient access
- p.869: To modify the facet, you always have to override the corresponding protected functions.
         Defining functions with the same interface as the public facet functions,
         would only overload them as they are not virtual.
- p.870: iostream operators delegate the conversion to the facets of the locale::numeric category
- p.884: time_put -- time and date formatting based on strftime() in time.h
- p.885: time conversion specifiers
- p.886: print time/date, using std::chrono::system_block::now()
- p.889: date_order
- p.893: string to wstring, wstring to string
- p.896: islower, isdigit, isspace, ... -- C-functions
            "The C++ convenience functions should not be used in code sections where performance is crucial.
             It is much faster to obtain the corresponding facet from the locale
             and to use the functions on this object directly"
- p.896: see footnote: how C and C++ locale might differ
- p.898: how the conversion by 'codecvt' is different from the one by 'widen()/narrow() of ctype'
- p.901: utf8_to_wstring
- p.903: utf8 –> wchar_t –> utf16
- p.905: perror and errno
- p.907: <random>: randomness, distributions
            distributions, engines (sources of randomness)

            engines create random unsigned values,
            which are uniformly distributed between a predeﬁned min and a max;
            the distributions transfer those values into random numbers,
            which are linearly or nonlinearly distributed according to user-supplied parameter
- p.909: engines & distributions
         - engines:
            - stateful sources of randomness
            - functors, create unsigned values uniformly distributed in predefined [min,max] / [min,max)
         - distributions:
            - create random numbers out of engines' random values according to user-supplied params
- p.910: pass seed to engine, shuffle vector
- p.911: don't pass temporary engine
- p.913: all 16 random-number engines
- p.914-915: save and restore state of random engine using << and >>
- p.917: distributions overview
- p.926: complex numbers, polar coordiantes
- p.927: while (cin.peek() != EOF) { ... }
- p.934: copy stream's flags, locale, precision
- p.941: numeric functions (inherited from C, form math.h and stdlib.h)
- p.941: div() returns quotient and remainder of int division
- p.946: high-level interface: std::async(), std::future<>
         - async expects a callable object (function, member function, functor, lambda)
- p.949: when calling get() of std::future, 1 of 3 things might happen:
            1) if func was started with async(), in a separate thread and has already finished,
               you immediately get the result
            2) if func was started but has not yet finished,
               get() blocks and waits for its end and yields the result
            3) if func was not started yet, it will be forced to start now and,
               like a synchronous function call, get() will block until it yields the result
- p.950: example of bad usage:
         –––
         std::future<int> res1(std::async(f1));
         int result = f2() + res1.get();
         –––
         => as evaluation of r=a+b is unspecified (either a or b could be evaluated first),
            if res1.get() is called before f2(), we have sequential processing!
- p.950: To have the best effect, in general, your goal should be to
         maximize the distance between calling async() and calling get().
- p.951: tell std::async not to defer execution, but start when called
            launch policy: std::launch:async, std::launch:deferred
- p.951: lazy evaluation using std::launch:deferred
- p.953: can only once call future<>::get()
- p.953: functions of std::future:
            wait        // forces thread to start      -
            wait_for    // don't force thread to start -
            wait_until  // don't force thread to start -
- p.954: use wait_for/wait_unitl for speculative execution
         (launch both quickComp and accurateComp and take best result available after some time)
- p.954: careful at what scope future<> is declared! dtor blocks until async task has finished! <––– not good... (see also p.976)
- p.955: check if task was deferred: 'f.wait_for(std::chrono::seconds(0)) != std::future_status::deferred'
- p.959: if using async, pass all objects BY VALUE !!!
         - ways of calling async
               async(f)
               async(lambda)
               async(func_ptr,arg1,arg2,...)
               async(mem_func,obj,arg1,arg2,...)
- p.960: get() can only be called once, BUT:
         std::shared_future<> to process outcome of concurrent computation multiple times (by multiple other threads)
- p.961: pass std::shared_future<> to several async threads
- p.963: T future<T>::get() is move-optimized, const T& shared_future<T>::get() is access-optimized
- p.964: low-level interface: std::thread t(...), t.join(), std::promise
         - caller of thread calls join():   wait for the thread
         - caller of thread calls detach(): detach from thread and let it run in bg without control
         - if you let the thread run in the bg and main() ends, all threads are terminated abruptly
- p.965: std::thread example
- p.967: detached threads accessing non-local resources
         - The lifetime problem also applies to global and static objects,
           because when the program exits, the detached thread might still run,
           which means that it might access global or static objects that are already destroyed
           or under destruction. This would result in UB.
- p.969: - to pass values to threads: pass them as arguments, e.g.: 'std::thread t1(do_work, '.');'
         - to get a result from thread
            - can pass return arguments by ref
            - can use std::promise (promise is counterpart of future)
               => future allows to retrieve data using get()
               => promise allows to provide data using set_*()
- p.970: std::promise
- p.972: packaged_task (if I don't want to start immediately, thread pool)
- p.973: thread interfaces overview
- p.974: async and its 2 forms of launch policy
- p.975: future<> in detail
- p.976: shared_future<> in detail
- p.977: promise<> in detail
- p.977: packaged_task<> in detail
- p.979: std::thread in detail
         - MUST call either join() or detach()
- p.982: synchronization techniques: mutexes & locks, condition variables, atomics
         - without synchronization, concurrent access only safe when all threads only read shared data
- p.983: "the order of argument evaluation of a function call is unspeciﬁed"
- p.985: guarantees by C++ standard:
         - concurrent access to **different elements** is fine
           BUT note: beware of false sharing!!!
         - for streams
            - sync_with_stdio()==false disables concurrency support for standard streams!! (see also p.845)
            - using standard streams with sync_with_stdio()==true is ok
            - concurrent access to string stream, file stream, stream buffer is UB
- p.986: problem of reordering statements
- p.987: deal with 'atomicity' & 'order' in C++
            futures & promises
            mutexes and locks
            condition variables
            atomic types/atomics, high-level
            atomic types/atomics, low-level
- p.989: all places have to use the same mutex (i.e. has to be global/static)
- p.990: don't lock & unlock mutex, but rather use std::lock_guard<std::mutex> (in braces/scope)
- p.992: monitor / recursive locks
- p.993: std::recursive_mutex
- p.994: try_lock, try_lock_for, try_lock_until, std::timed_mutex, std::recursive_timed_mutex
         - use std::lock_guard<std::mutex> lg(mtx,std::adopt_lock) after mtx.try_lock(),
           as mtx passed to lock_guard ctor already locked (see also p.1000)
         - try_lock might fail spuriously !
- p.995: lock multiple mutexes
- p.996: std::unique_lock<std::mutex> (similar to unique_ptr), more flexible than std::lock_guard<std::mutex>
- p.997: example (polling) of 2 threads (one is preparing sth for the other), using unique_lock and lock_guard
- p.998: mutex overview
         - if mutex is locked, then other lock() will block until the mutex is available again
- p.999: lock_guard overview
- p.1001: unique_lock overview
- p.1001: std::once_flag, std::call_once
            std::once_flag oc;
            std::call_once(oc, []{ init_code() });
- p.1003: condition variable
- p.1004: for a condition variable, need mutex + unique_lock
- p.1004: condition_variable
            1) a thread that signals the fulﬁllment of a condition has to call
                  readyCondVar.notify_one(); // notify one of the waiting threads
               or
                  readyCondVar.notify_all(); // notify all the waiting threads
            2) any thread that waits for the condition has to call
                  std::unique_lock<std::mutex> l(readyMutex);
                  readyCondVar.wait(l); // + protection against spurious wakeups!
            (see 1005 for complete example)
- p.1005: for a condition variable, need
            ready_flag
            mutex (+ unique_lock)
            condition_variable
- p.1006: condition_variable
            readyCondVar.wait(ul, []{ return readyFlag; });
               is equivalent to
            while (!readyFlag) { readyCondVar.wait(ul); }
- p.1006: example with global queue, concurrently used (multiple providers & consumers)
- p.1009: usage case for notify_all
- p.1010: "All threads waiting for a condition variable have to use the same mutex,
           which has to be locked by a unique_lock when one of the wait() members is called.
           Otherwise, UB occurs."
- p.1010: the waiting functions temporarily unlock the mutex performing the following three atomic steps:
            (see p.1010)
- p.1012: atomics guarantee by default 'sequential consistency', atomic bool-flag
          - std::atomic<bool> instead of bool + mutex
          - e.g.: std::atomic<bool> readyFlag(false); ... readyFlag.store(true); ... readyFlag.load();
- p.1013-1014: compare use of atomic falg to before with mutex and non-atomic flag
               => for condition variable, still need mutex
- p.1016: store/load determine, when modifiations on shared memory become visible in other thread(s)
          - store -> release, load -> aquire
          - default memory order: memory_order_seq_cst
          - primary class template, explicit specializations
- p.1017: atomics operations table
- p.1018: is_lock_free, to check if I have native HW support for atomic operations
          compare-and-swap (CAS) operations
          CAS pseudo-code
- p.1019: C-style atomics' interface
- p.1019: @end of page: resources for C++ memory model/memory order
- p.1021: memory_order_release, memory_order_aquire
- p.1022: use case for memory_order_relaxed
          atomic_thread_fence(), atomic_signal_fence()
- p.1024: - alias templates (template typedefs)
          - user-defined allocator
          - possible impl of default allocator
- p.1028: possible implementation of std::vector::reserve
- p.1031: forums about C++
- p.1111: use of proxy in std::bitset
- p.1112: std::bitset<N_bits>::to_string()
            "The order of the characters is equivalent to the order of the bits with descending index"
- p.1114: numeric processing with 'Blitz'
- p.1118-1121: std::valarray can be used similar to python numpy-arrays
- p.1121: std::valarray: lazy evaluation, reference semantics
- p.1121: slices: (start_idx,N_elems,stride), e.g.: stride(2,4,3) == {2,5,8,11}
- p.1151: placement new, used in Allocator::construct -- 'new(ptr) T(val)' (see also p.1158-1161)
- p.xxx: >> NEXT <<
   
*********************************************************************************************************
STL containers (figure p.167)
   sequence containers     (ordered collections)      // array or linked list
      array
      vector
      deque          // double-ended queue
      list           // doubly linked list
      forward_list   // singly linked list
   associative containers  (sorted collections)       // balanced binary tree (p.315: red-black trees)
      set/multiset
      map/multimap
   unordered containers    (unordered collections)    // hash tables (p.180: internally: array of linked lists)
      unordered_set/unordered_multiset
      unordered_map/unordered_multimap

other containers
   strings
   ordinary C-style arrays
   user-defined containers
   note: because of begin and end, circular container types like ringbuffers don't really fit into the STL framework

container adapters
   stack             // LIFO
   queue             // FIFO
   priority_queue    // the next element is always one the highest priority inside the queue

iterators (p.188)
   - p.189: All container classes provide the same basic member functions that enable them to use iterators
          to navigate over their elements. The most important of these functions are:
            begin()     cbegin()
            end()       cend()
            rbegin()    crbegin()
            rend()      crend()
         => begin() and end() define a half-open range: [begin, end)
   - iterator types:
      <container>::iterator
      <container>::const_iterator
   - p.191: prefer ++pos over pos++ (see also p.436)
   - iterator categories
      - forward iterator           // can only iterator forwards
         => forward_list, unordered_set, unordered_multiset, unordered_map, unordered_multimap
      - bidirectional iterator     // can iterate both forwards and backwards
         => list, set, multiset, map, multimap
      - random-access iterator     // can iterate both forwards and backwards + random access + provide ops for iterator arithmetic akin to ptr arithmetic
         => vector, deque, array, iterators of strings
      - input iterators
      - output iterators

iterator adapters (p.210)
   Insert iterators
      back_inserter     // uses: push_back(val)  // for: vector, deque, list, string
      front_inserter    // uses: push_front(val) // for: deque, list, forward_list
      inserter          // uses: insert(pos,val) // for: all containers, except array and forward_list, i.e. also for associative and unordered containers
   Stream iterators
      istream_iterator
      ostream_iterator
   Reverse iterators
      // for: all containers with bidirectional iterators or random access iterators,
      //      i.e.: all sequence containers except forward_list, and all associative containers
      rbegin()    crbegin()
      rend()      crend()
   Move iterators

normal/forward vs. reverse iterators
   +~~~+~~~+~~~+~~~+~~~+~~~+~~~+                +~~~+~~~+~~~+~~~+~~~+~~~+~~~+
   | 1 | 2 | 3 | 4 | 5 | 6 | 7 |                | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
   +~~~+~~~+~~~+~~~+~~~+~~~+~~~+                +~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     |                           |            |                           |
   begin                        end         rend                        rbegin
            [begin,end)                               [rbegin,rend)

   => the referenced value of an iterator changes when switching between normal and reverse iterators !!! (see 448)

situations where iterators might get invalidated
   p.246: iterators become invalid
          - for vectors & deques, if elements are inserted or deleted or reallocation takes place
          - for unordered containers, if rehashing takes place (which also might be a result of an insertion)
   p.260: 
      - All containers except vectors and deques guarantee that iterators and references to elements
        remain valid if other elements are deleted. 
        For vectors, only the elements before the point of erase remain valid.
      - If you remove all elements by using clear(), for vectors, deques, and strings
        any past-the-end iterator returned by end() or cend() may become invalid
      - If you insert elements, only lists, forward lists, and associative containers guarantee
        that iterators and references to elements remain valid.
        For vectors, this guarantee is given if insertions don’t exceed the capacity.
        For unordered containers, that guarantee is given to references in general but to iterators only
        when no rehashing happens, which is guaranteed as long as with insertions
        the number of resulting elements is less than the bucket count times the maximum load factor.
   p.271: if you exceed the capacity() of a vector, reallocation happens
      - reallocation invalidates all references, pointers, and iterators for elements of the vector
      - Because the capacity of vectors never shrinks, it is guaranteed that references, pointers, and
        iterators remain valid even when elements are deleted,
        provided that they refer to a position before the manipulated elements.
        However, insertions invalidate all references, pointers, and iterators when the capacity gets exceeded.
   p.276: std::vector
      - inserting or removing elements invalidates references, pointers, and iterators that refer to the following elements
      - an insertion that causes reallocation invalidates all references, iterators, and pointers
   p.284: std::deque
      - any insertion or deletion of elements other than at the beginning or end
        invalidates all pointers, references, and iterators that refer to elements of the deque
      - reallocation may perform better than for vectors because according to their typical internal structure,
        deques don’t have to copy all elements on reallocation
      - blocks of memory might get freed when they are no longer used, so the memory size of a deque might shrink


properties of STL containers
   sequence containers
      array
         - random access // O(1)
         - fixed size at compile time
      vector
         - random access // O(1)
         - appending/inserting and removing elements at the end is very fast // O(1)
         - inserting and deleting elements in the middle or at the beginning is slow
      deque
         - random access // O(1)
         - inserting and removing elements at the end AND at the beginning is fast // p.284: O(1)
         - inserting and deleting elements in the middle is slow
      list
         - no random access (must follow the chain of the elements' links to access a specific element)
            - step to next or previous element is O(1)
            - general access to arbitrary element is slow O(n)
         - insertion or removal of an element at any position is fast (p.291: provided you are there) (only links must be changed)
            => moving an element in the middle of a list is very fast compared to vector/deque
         - accessing 1st or last element ist fast O(1) (p.291)
         - insert and delete an element is always O(1), once you are there (p.291)
      forward_list
         - no random access
         - no push_back!
      -- valid for all sequence containers --
         - finding an element with a specific value is rather slow: O(n)
   associative containers
      set/multiset / map/multimap
      - finding an element with a specific value (or element access) is rather fast: O(log(n))
      - can't modify values directly
      - 'map' can be used as an associative array
      - 'multimap' can be used as a dictionary
      - multiset: duplicates are allowed
      - multimap: may contain multiple elements that have the same key
      - p.180: C++11 guarantees that newly inserted elements are inserted at the end of equivalent elements
               that multisets and multimaps already contain
   unordered containers
      unordered_set/unordered_multiset / unordered_map/unordered_multimap
      - ﬁnding an element with a speciﬁc value (or element access) is even faster than for associative containers: it is O(1)
      - unordered_multiset: duplicates are allowed
      - unordered_multimap: may contain multiple elements that have the same key
      - 'unordered_map' can be used as an associative array
      - 'unordered_multimap' can be used as a dictionary
      - p.183: "to ensure that you can delete elements while processing all elements,
               the standard guarantees that deleting elements does not cause a rehashing.
               But an insertion after a deletion might cause rehashing. For details, see Section 7.9, page 355"

algorithms
   find
   copy
   remove
   remove_if
   replace_if
   find_if: search for the 1st element of the given range for which the passed unary predicate yields true // p.227
      find_if(begin, end, pred) // returns it to 1st elem for which pred is true, or end

TODO: list of all member functions and which STL containers have them
____________________________________________________________________________________________________________________________________________________
|            sequence containers               |   associative containers    |                         unordered containers                        |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+
| array | vector | deque | list | forward_list | set/multiset | map/multimap | unordered_set/unordered_multiset | unordered_map/unordered_multimap |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+
|       |        |       |      |              |              |              |                                  |                                  |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+
|       |        |       |      |              |              |              |                                  |                                  |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+
|       |        |       |      |              |              |              |                                  |                                  |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+
|       |        |       |      |              |              |              |                                  |                                  |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+
|       |        |       |      |              |              |              |                                  |                                  |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+
|       |        |       |      |              |              |              |                                  |                                  |
+——————————————————————————————————————————————+—————————————————————————————+—————————————————————————————————————————————————————————————————————+

T c               default-ctor
T c(c2)           copy-ctor
T c = c2          copy-ctor
T c(rv)           move-ctor
T c = rv          move-ctor
T c(beg,end)
T c(initlist)
T c = initlist
c.~T()            dtor

c.empty()
c.size()
c.max_size()

c1 == c2
c1 != c2
c1 < c2
c1 > c2
c1 <= c2
c1 >= c2

c = c2
c = rv
c = initlist

c1.swap(c2)
swap(c1,c2)

c.begin()
c.end()
c.cbegin()
c.cend()
c.clear()

std::array
   array<Elem,N> c               default-ctor
   array<Elem,N> c(c2)           copy-ctor
   array<Elem,N> c = c2          copy-ctor
   array<Elem,N> c(rv)           move-ctor
   array<Elem,N> c = rv          move-ctor
   array<Elem,N> c = initlist
   
   c.empty()
   c.size()
   c.max_size()
   
   c1 == c2
   c1 != c2
   c1 < c2
   c1 > c2
   c1 <= c2
   c1 >= c2
   
   c = c2
   c = rv
   c.fill(val)
   
   c1.swap(c2)
   swap(c1,c2)
   
   c[idx]
   c.at(idx)
   c.front()
   c.back()
   
   c.begin()
   c.end()
   c.cbegin()
   c.cend()
   c.rbegin()
   c.rend()
   c.crbegin()
   c.crend()

std::vector
   vector<Elem> c               default-ctor
   vector<Elem> c(c2)           copy-ctor
   vector<Elem> c = c2          copy-ctor
   vector<Elem> c(rv)           move-ctor
   vector<Elem> c = rv          move-ctor
   vector<Elem> c(N)
   vector<Elem> c(N,elem)
   vector<Elem> c(beg,end)
   vector<Elem> c(initlist)
   vector<Elem> c = initlist
   c.~vector()                   dtor
   
   c.empty()
   c.size()
   c.max_size()
   
   c.capacity()
   c.reserve()
   c.shrink_to_fit()
   
   c1 == c2
   c1 != c2
   c1 < c2
   c1 > c2
   c1 <= c2
   c1 >= c2
   
   c = c2
   c = rv
   c = initlist
   c.assign(n,elem)
   c.assign(beg,end)
   c.assign(initlist)
   
   c1.swap(c2)
   swap(c1,c2)
   
   c[idx]
   c.at(idx)
   c.front()
   c.back()
   
   c.begin()
   c.end()
   c.cbegin()
   c.cend()
   c.rbegin()
   c.rend()
   c.crbegin()
   c.crend()

   c.push_back()
   c.pop_back()
   c.insert(pos,n,elem)
   c.insert(pos,beg,end)
   c.insert(pos,initlist)
   c.emplace(pos,args...)
   c.emplace_back(args...)
   c.erase(pos)
   c.erase(beg,end)
   c.resize(num)
   c.resize(num,elem)
   c.clear()
std::deque
   similar to std::vector, but
   - c.capacity()
   - c.reserve()
   + c.push_front()
   + c.pop_front()
   + c.emplace_front(args...)
std::list
   similar to std::deque, but
   - c[idx]
   - c.at(idx)
   + c.remove(val)
   + c.remove_if(op)
   + table 7.25
std::forward_list
   similar to std::list, but
   - rbegin(), rend(), crbegin(), crend()
   - c.size()
   - back(), push_back(), pop_back()
   + insert_after()
   + before_begin(), cbefore_begin()
   ~ splice_after instead of splice
std::set / set::multiset
   + set<Elem,Op>
   + c.count(val)       | O(log(n))
   + c.find(val)        | O(log(n))
   + c.lower_bound(val) | O(log(n))
   + c.upper_bound(val) | O(log(n))
   + c.equal_range(val) | O(log(n))
   + c.insert(...)
   + c.emplace(...)
   + c.erase(...)
std::map / set::multimap
   + map<Key,Val>
   + map<Key,Val,Op>
   + ...
   + c[key]
   + c.at(key)
unordered_*
   + T c(beg,end,n_buckets)
   + T c(beg,end,n_buckets,hash_func)
   + T c(beg,end,n_buckets,hash_func,cmp)
   
   + unordered_set<Elem>
   + unordered_[multi]set<Elem,Hash>
   + unordered_[multi]set<Elem,Hash,Cmp>
   
   + unordered_map<Key,Val>
   + unordered_[multi]map<Key,Val,Hash>
   + unordered_[multi]map<Key,Val,Hash,Cmp>
   
   + c.bucket_count()
   + c.max_bucket_count()
   + c.load_factor()
   + c.max_load_factor()
   + c.max_load_factor(val)
   + c.rehash(n_buckets)
   + c.reserve(n_elements)
   
   + c.count(val)
   + c.find(val)
   + c.equal_range(val)
*********************************************************************************************************/

#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <vector>
#include <iomanip>      // setw
#include <cstring>      // memcpy
#include <limits>       // numeric_limits
#include <functional>   // reference_wrapper, ref, hash<T>, negate, multiplies
#include <time.h>       // clock_gettime, CLOCK_MONOTONIC_RAW, struct timespec
#include <unistd.h>     // usleep
#include <memory>       // smart-ptrs: shared_ptr, unique_ptr, weak_ptr
#include <list>
#include <iterator>     // ostream_iterator
#include <set>
#include <deque>
#include <forward_list>
#include <map>
#include <unordered_set>
#include <random>       // default_random_engine
#include <bitset>
#include <cassert>
#include <fstream>      // ofstream
#include <locale>
#include <chrono>
#include <codecvt>
#include <future>       // async, future
//#include <thread>       // this_thread
#include <mutex>

auto print_hline = []() { cout << std::string(40,'~') << endl; };

// p.17
template <typename T>
void print_elements(const T& coll, const std::string& optstr="")
{
   if (coll.empty()) {
      cout << optstr << "\n";// << "[]\n";
   } else {
      cout << optstr;// << "[";
      for (const auto& e : coll) {
         cout << e << ", ";
      }
      cout << "\b\b \n";
   }
}

// p.74: print any tuple with the standard output operator<<
// helper: print element with index idx of tuple with max elements
template <int idx, int max, typename... Types>
struct PrintTuple {
   static void print(std::ostream& os, const std::tuple<Types...>& t) {
      os << std::get<idx>(t) << (idx+1==max ? "" : ",");
      PrintTuple<idx+1, max, Types...>::print(os, t);
   }
};
// partial specialization to end recursion
template <int max, typename... Types>
struct PrintTuple<max, max, Types...> {
   static void print(std::ostream& os, const std::tuple<Types...>& t) {
      (void) os;
      (void) t;
   }
};
// output operator for tuples
template <typename... Types>
std::ostream& operator<<(std::ostream& os, const std::tuple<Types...>& t) {
   os << "{";
   PrintTuple<0, sizeof...(Types), Types...>::print(os, t);
   os << "}";
   return os;
}

/*
   testing stuff related to smart pointers (shared_ptr,unique_ptr,weak_ptr)

   note:
   - default deleter provided by shared_ptr calls 'delete', not 'delete[]' !!!
     => so if I allocate array, HAVE TO define own deleter !!! (or use 'std::default_delete<int[]>()')
   - instead of providing custom deleter for unique_ptr to call delete[],
     use partial specialization of class unique_ptr to deal with arrays
   - custom deleter is provided to unique_ptr as 2nd template argument
*/
void testing_smart_pointers()
{
   {
      // p.77
      cout << "smart ptrs tests (1)\n";
      std::shared_ptr<std::string> pStr1(new std::string("some str 1"));
      std::shared_ptr<std::string> pStr2 = std::make_shared<std::string>("some str 2");
      std::shared_ptr<std::string> pStr3;
      //pStr3 = std::make_shared<std::string>("some str 2");
      pStr3.reset(new std::string("some str (resetting shared_ptr)"));

      std::vector<std::shared_ptr<std::string>> vec;
      vec.push_back(pStr1);
      vec.push_back(pStr2);
      vec.push_back(pStr3);
      vec.push_back(pStr2);
      cout << "pStr2 use_count: " << pStr2.use_count()  << "\n";
      cout << "pStr2 use_count: " << vec[1].use_count() << "\n";
      cout << "pStr2 use_count: " << vec[3].use_count() << "\n";
   }
   {
      // p.80
      cout << "smart ptrs tests (2)\n";
      std::shared_ptr<std::string> pStr1(new std::string("some str 1"),
         [](std::string *p) {
            cout << "delete " << *p << "\n";
            delete p;
         });
      cout << "use_count after init: " << pStr1.use_count() << "\n";
      auto pStr2 = pStr1;
      cout << "use_count after 2: " << pStr1.use_count() << "\n";
      auto pStr3 = pStr1;
      cout << "use_count after 3: " << pStr1.use_count() << "\n";
      pStr1 = nullptr;
      cout << "after setting shared_ptr to nullptr,  use_count: " << pStr1.use_count() << "\n";
      pStr2 = pStr3 = nullptr;
      cout << "after setting all remaining shared_ptr to nulltpr\n";

      auto deleter = [](std::string *p) {
         cout << "own deleter - deleting " << *p << "\n";
         delete p;
      };
      std::shared_ptr<std::string> pStr_own_deleter(new std::string("some str 2"), deleter);
      
      cout << "smart ptrs tests (3) (arrays)\n";
      std::shared_ptr<int> p1(new int[10], [](int *p){delete[] p;});
      std::shared_ptr<int> p2(new int[10], std::default_delete<int[]>());
   }
   {
      cout << "smart ptrs tests (4) (weak_ptr)\n";
      std::shared_ptr<std::string> sp = std::make_shared<std::string>("str shared ptr");
      std::weak_ptr<std::string> wp = sp;
      cout << "wp.expired(): " << wp.expired() << "\n";
      cout << wp.lock()->c_str() << "\n";
      sp.reset();
      cout << "wp.expired(): " << wp.expired() << "\n";
   }
   {
      cout << "smart ptrs tests (5) (unique_ptr)\n";
      std::unique_ptr<std::string> up(new std::string("str unique ptr"));
      cout << *up << "\n";
      (*up)[0] = 'S';
      cout << *up << "\n";

      cout << up.get() << "\n";
      auto str_ptr = up.release(); // up loses ownership!
      cout << up.get() << "\n";
      delete str_ptr;
   }
   {
      cout << "smart ptrs tests (6) (unique_ptr)\n";
      std::unique_ptr<std::string> up = std::make_unique<std::string>("str unique ptr"); // since C++14
      cout << *up << "\n";
      (*up)[0] = 'S';
      cout << *up << "\n";
      cout << "up is nullptr: " << (up.get()==nullptr) << ", if(up): " << (up ? true : false) << "\n";
      up.reset();
      cout << "up is nullptr: " << (up.get()==nullptr) << ", if(up): " << (up ? true : false) << "\n";
   }
   {
      cout << "smart ptrs tests (7) (unique_ptr)\n";
      std::unique_ptr<std::string> up = std::make_unique<std::string>("str unique ptr"); // since C++14
      cout << *up << "\n";
      (*up)[0] = 'S';
      cout << *up << "\n";
      cout << "up is nullptr: " << (up.get()==nullptr) << ", if(up): " << (up ? true : false) << "\n";
      // transfer ownership of up
      std::unique_ptr<std::string> up_newOwner(std::move(up));
      cout << "up is nullptr: " << (up.get()==nullptr) << ", if(up): " << (up ? true : false) << "\n";
      cout << *up_newOwner << "\n";

      std::unique_ptr<std::string> up_1 = std::make_unique<std::string>("just some string no1");
      std::unique_ptr<std::string> up_2 = std::make_unique<std::string>("just some string no2");
      cout << "up_1 is nullptr: " << (up_1.get()==nullptr) << ", if(up_1): " << (up_1 ? true : false) << "\n";
      up_2 = std::move(up_1);
      cout << "up_1 is nullptr: " << (up_1.get()==nullptr) << ", if(up_1): " << (up_1 ? true : false) << "\n";
   }
   
   /*
      The transfer of ownership implies a special use for unique_ptrs, i.e.,
      functions can use them to transfer ownership to other functions.
      This can occur in two ways:
      (1): A function can behave as a **sink** of data
            - unique_ptr is passed as argument to the function by rvalue-ref created with std::move
               => function parameter takes ownership
            - if the function does not transfer it again, the object gets deleted on function exit
      (2): A function can behave as a **source** of data
            - unique_ptr is returned -- ownership is transferred to caller
            note: this pattern can be used in a factory function,
                  that returns objects created on the heap.
   */
   {  // (1)
      auto sink = [](std::unique_ptr<int> up) -> void {
         cout << "sink takes ownership of unique_ptr\n";
         (void) up; // to silence compiler warning
         // ...
      };
      std::unique_ptr<int> p = std::make_unique<int>(1000);
      sink(std::move(p));
   }
   {  // (2)
      auto source = []() -> std::unique_ptr<int> {
         std::unique_ptr<int> ptr(new int);
         cout << "source transfers ownership of unique_ptr to caller\n";
         return ptr;
      };
      std::unique_ptr<int> p;
      p = source();
   }

   {
      cout << "smart ptrs tests (8) (unique_ptr and arrays)\n";
      {
         std::unique_ptr<int[]> up(new int[3]);
         cout << up[0] << "\n";
         cout << up[1] << "\n";
         cout << up[2] << "\n";
      }
      {
         std::unique_ptr<std::string[]> up(new std::string[3]);
         up[0] = "1st string";
         up[1] = "-:|:-";
         up[2] = "3rd str";
         cout << up[0] << "\n";
         cout << up[1] << "\n";
         cout << up[2] << "\n";
      }
   }
   {
      cout << "smart ptrs tests (9) (unique_ptr and arrays)\n";
      std::unique_ptr<int[]> up(new int[3]{11,22,33});
      cout << up[0] << "\n";
      cout << up[1] << "\n";
      cout << up[2] << "\n";
   }
   {
      cout << "smart ptrs tests (10) (unique_ptr custom deleter)\n";
      std::unique_ptr<int, void(*)(int*)> up(new int[3]{11,22,33},
         [](int *p){ cout << "custom unique_ptr-deleter !·_·!\n"; delete[] p; });
      cout << *(up.get()+0) << "\n";
      cout << *(up.get()+1) << "\n";
      cout << *(up.get()+2) << "\n";
   }
}
// transfer of ownership by means of unique_ptr
// (1) usage: sink(std::move(up))
void sink(std::unique_ptr<int> up)
{
   // ...
   (void) up; // to silence compiler warning
}
// (2) usage: auto up = source()
std::unique_ptr<int> source()
{
   std::unique_ptr<int> ptr(new int);
   return ptr;
}

void testing_const()
{
   int a = 42;
   cout << "a=" << a << "\n";
   
   const int& r1 = a;
   cout << "r1=" << r1 << "\n";
   // r1++; // read-only reference
   cout << "r1=" << r1 << "\n";
   
   int const& r2 = a;
   cout << "r2=" << r2 << "\n";
   //r2++; // read-only reference
   cout << "r2=" << r2 << "\n";

   //int &const r3 = a; // doesn't work -- make sense, as reference is always "const" (can't reassign)
   //int const &const r3 = a;

   const int b = 11;
   //int& ref = b; // doesn't work -- binding 'int&' to 'const int' discards qualifiers (makes sense)
   const int& ref = b;
   (void) ref;

   int& r3 = a;
   r3 = b;
}


/*
   Testing some virtual-related stuff
   - virtual dtor
   - virtual member functions
*/
class Base {
public:
   Base();                             // default ctor
   ~Base();                            // dtor
   Base(const Base& other);            // copy ctor
   Base(Base&& other);                 // move ctor
   Base& operator=(const Base& other); // copy assignment operator
   Base& operator=(Base&& other);      // move assignment operator

   void print_nonVirtual() { cout << "non-virtual print BASE\n"; }
   virtual void print_Virtual() { cout << "virtual print BASE\n"; }
public:
   int data;
};
Base::Base()  { cout << std::setw(16) << std::left << "Base ctor" << "[" << this << "]\n"; }
Base::~Base() { cout << std::setw(16) << std::left << "Base dtor" << "[" << this << "]\n"; }

class Derived : public Base {
public:
   Derived();
   ~Derived();

   void print_nonVirtual() { cout << "non-virtual print DERIVED\n"; }
   void print_Virtual() { cout << "virtual print DERIVED\n"; }
};
Derived::Derived()  { cout << std::setw(16) << std::left << "Derived ctor" << "[" << this << "]\n"; }
Derived::~Derived() { cout << std::setw(16) << std::left << "Derived dtor" << "[" << this << "]\n"; }

// for testing of calling function that takes Base with Derived
void f_takes_Base(Base& b)
{
   b.print_nonVirtual();
   b.print_Virtual();
}
void testing_function_taking_Base()
{
   Base b;
   Derived d;
   cout << "calling f with Base:\n";
   f_takes_Base(b);
   cout << "calling f with Derived:\n";
   f_takes_Base(d);
}
void testing_virtual()
{
   {
      Base *basePtr;
      Derived d;
      basePtr = &d;
      cout << "basePtr->print_nonVirtual():\n  ";
      basePtr->print_nonVirtual();
      cout << "basePtr->print_Virtual():\n  ";
      basePtr->print_Virtual();
   }
   cout << "–––\n";
   {
      Base b;
      b.print_nonVirtual();
      b.print_Virtual();
   }
   cout << "–––\n";
   {
      Derived d;
      d.print_nonVirtual();
      d.print_Virtual();
   }
   cout << "–––\n";
}

void testing_reference_wrapper()
{
   //std::vector<Base&> v; // doesn't work!!!
   std::vector<std::reference_wrapper<Base>> v;
   Base b;
   b.data = 111;
   cout << "b.data=" << b.data << "\n";
   v.push_back(b);
   v[0].get().data = 222;
   cout << "b.data=" << b.data << "," << v[0].get().data << "\n";

   std::vector<Base*> v_of_ptrs;
   v_of_ptrs.push_back(&b);
   v_of_ptrs[0]->data = 333;
   cout << "b.data=" << b.data << "," << v_of_ptrs[0]->data << "\n";
}

long nanos() {
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
   return ts.tv_sec*1000000000L + ts.tv_nsec;
}
// https://stackoverflow.com/questions/13772567/how-to-get-the-cpu-cycle-count-in-x86-64-from-c
uint64_t rdtsc() {
   unsigned int lo,hi;
   __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
   return ((uint64_t)hi << 32) | lo;
}
void testing_timing()
{
   cout << "testing nanos:\n";
   cout << "before:\n";
   long n1 = nanos();
   uint64_t rdtsc1 = rdtsc();
   
   // payload
   #if 0
   for (size_t i=0; i < 20000LL; ++i) {
      std::vector<std::string> v(i);
   }
   #endif
   
   uint64_t rdtsc2 = rdtsc();
   long n2 = nanos();
   long diff = n2-n1;
   uint64_t diff_rdtsc = rdtsc2-rdtsc1;
   cout << "start      : " << n1 << "\n";
   cout << "end        : " << n2 << "\n";
   cout << "diff       : " << diff << " ns -- i.e.: " << diff/1000000000 << "s and " << diff%1000000000 << " ns\n";
   cout << "rdtsc1     : " << rdtsc1 << "\n";
   cout << "rdtsc2     : " << rdtsc2 << "\n";
   cout << "diff_rdtsc : " << diff_rdtsc << " ns\n";

   long seconds2Wait = 2;
   cout << "waiting for " << seconds2Wait << " seconds:\n" << std::flush;
   long start = nanos();
   while ( (nanos()-start) < (seconds2Wait*1000000000L) ) {
      usleep(1);
   }
   n2 = nanos();
   cout << "waited for " << n2-start << " ns\n";
   cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << "\n";
}

struct ns_split_in_units {
   long m_ns;
   explicit ns_split_in_units(long ns) : m_ns(ns) {}
   friend std::ostream& operator<<(std::ostream& os, const ns_split_in_units& obj) {
      os <<   obj.m_ns/1000000000 << "s, "
         << ( obj.m_ns%1000000000)/1000000 << " ms, "
         << ((obj.m_ns%1000000000)%1000000)/1000 << " us, "
         << ((obj.m_ns%1000000000)%1000000)%1000 << " ns";
      return os;
   }
};
void testing_vector_capacity()
{
   /*auto print_ns_split_in_units = [](long ns) {
      cout <<   ns/1000000000 << "s, "
           << ( ns%1000000000)/1000000 << " ms, "
           << ((ns%1000000000)%1000000)/1000 << " us, "
           << ((ns%1000000000)%1000000)%1000 << " ns\n";
   };*/
   {
      long n1, n2, diff;

      n1 = nanos();
      std::vector<int> v1;
      n2 = nanos();
      diff = n2-n1;
      cout << "testing creating empty vector\n";
      cout << "start : " << n1 << "\n";
      cout << "end   : " << n2 << "\n";
      cout << "diff  : " << diff << " ns -- i.e.: " << ns_split_in_units(diff) << "\n";

      for (int multiplicator : {1, 10, 100, 1000}) {
         int num_elements2init = 100*multiplicator;
         n1 = nanos();
         std::vector<int> v2(num_elements2init);
         n2 = nanos();
         diff = n2-n1;
         cout << "testing initializing vector with "<< num_elements2init << " elements\n";
         cout << "start : " << n1 << "\n";
         cout << "end   : " << n2 << "\n";
         cout << "diff  : " << diff << " ns -- i.e.: " << ns_split_in_units(diff) << "\n";

         std::vector<int> v3;
         int num_elements2reserve = 100*multiplicator;
         n1 = nanos();
         v3.reserve(num_elements2reserve);
         n2 = nanos();
         diff = n2-n1;
         cout << "testing reserving "<< num_elements2reserve << " elements\n";
         cout << "start : " << n1 << "\n";
         cout << "end   : " << n2 << "\n";
         cout << "diff  : " << diff << " ns -- i.e.: " << ns_split_in_units(diff) << "\n";
         cout << "\n";
      }
      /*
         => creating empty vector and calling reserve(N) seems to be faster than creating with N elements
            (as default ctor is called for v(N) !!!!)
      
                  std::vector<int> v(N);
                     vs.
                  std::vector<int> v;
                  v.reserve(N);
      */
   }
   {
      // p.276: remove all elements that have value val from vector
      std::vector<int> v = { 1, 2, 3, 4, 99, 5, 6, 99, 7, 8, 9, 99};
      int val2remove = 99;
      cout << "initially:\n";
      print_elements(v);
      
      v.erase(std::remove(v.begin(), v.end(), val2remove), v.end());
      cout << "after erasing all elements of value " << val2remove << " in vector:\n";
      print_elements(v);

      int pos2insert = 3;
      int val2insert = 100;
      auto it_pos2insert = v.begin() + pos2insert;
      auto ret = v.insert(it_pos2insert, val2insert);
      cout << "after inserting value " << val2insert << " @pos " << pos2insert << "\n";
      cout << "(ret==it_pos2insert): " << (ret==it_pos2insert) << "\n";
      print_elements(v);

      cout << "size/capacity BEFORE clear(): " << v.size() << "/" << v.capacity() << "\n";
      v.clear();
      cout << "size/capacity AFTER  clear(): " << v.size() << "/" << v.capacity() << "\n";
      cout << "\n";
   }
   {
      const int N = 10;
      /*
         std::vector<int> v(N);
      */
      {
         std::vector<int> v(N);
         cout << "initial          size/capacity: " << v.size() << "/" << v.capacity() << ", vector: ";
         print_elements(v);
         v.push_back(99);
         cout << "after push_back  size/capacity: " << v.size() << "/" << v.capacity() << ", vector: ";
         print_elements(v);
         cout << "\n";
      }
      /*
         std::vector<int> v;
         v.reserve(N);
      */
      {
         std::vector<int> v;
         cout << "initial          size/capacity: " << v.size() << "/" << v.capacity() << "\n";
         v.reserve(N);
         cout << "after reserve()  size/capacity: " << v.size() << "/" << v.capacity() << ", vector: ";
         print_elements(v);
         v.push_back(99);
         cout << "after push_back  size/capacity: " << v.size() << "/" << v.capacity() << ", vector: ";
         print_elements(v);
         cout << "\n";
      }
      /*
         =>
            std::vector<int> v(N);
            v[i] = ...;

            std::vector<int> v;
            v.reserve(N);
            v.push_back(...);
      */
   }
   {
      /*
         std::vector<int> v;
         v.resize(N)
      */
      const int N = 10;
      std::vector<int> v;
      cout << "initial       size/capacity: " << v.size() << "/" << v.capacity() << "\n";
      v.resize(N);
      cout << "after resize  size/capacity: " << v.size() << "/" << v.capacity() << ", vector: ";
      print_elements(v);
      for (size_t i = 0; i < v.size(); ++i) {
         v[i] = i+1;
      }
      cout << "after assigning v[i]=i+1: " << v.size() << "/" << v.capacity() << ", vector: ";
      print_elements(v);

      v.resize(N-2);
      cout << "after resize  size/capacity: " << v.size() << "/" << v.capacity() << ", vector: ";
      print_elements(v);

      v.resize(N+2);
      cout << "after resize  size/capacity: " << v.size() << "/" << v.capacity() << ", vector: ";
      print_elements(v);
      cout << "\n";
      /*
         =>
           std::vector<int> v;
            v.resize(N)
            v[i] = ...;
      */
   }
}

void test_operator_precedence()
{
   const char *str = "own strcpy";
   const char *p = str;
   cout << "strlen of str: " << strlen(str) << endl;
   while (*p) {
      cout << "|" << *p++ << "|\n";    // note: '*p++' is '(*p); p++;', and '*++p' would be '++p; (*p);'
   }
   
   char copy[32] = {0};
   char *p_cpy = copy;
   cout << "sizeof of copy: " << sizeof(copy) << endl;
   
   p = str;
   cout << "copy before: |" << copy << "|\n";
   while (*p) {
      *p_cpy++ = *p++;
   }
   cout << "copy after: |" << copy << "|\n";
}

void testing_range_forming_iterator_positions()
{
   // p.203

   {  // **random access iterators**
      std::vector<int> coll;
      // insert elements from 20 to 40
      for (int i=20; i<=40; ++i) { coll.push_back(i); }
      cout << "vec: "; for (const auto& e : coll) { cout << e << " "; } cout << "\n";

      // check if pos25 or pos35 comes first
      auto pos25 = std::find(coll.begin(), coll.end(), 25);
      auto pos35 = std::find(coll.begin(), coll.end(), 35);
      if (pos25 < pos35) {
         cout << "only [pos25,pos35) is valid (and non-empty)\n";
         cout << "  iterator distance: " << pos35-pos25 << "\n";
         cout << "  iterator distance: " << pos25-pos35 << "\n";
         cout << "  iterator distance: " << std::distance(pos25,pos35) << "\n";
         cout << "  iterator distance: " << std::distance(pos35,pos25) << "\n";
      }
      else if (pos35 < pos25) {
         cout << "only [pos35,pos25) is valid (and non-empty)\n";
      }
      else {
         cout << "pos35==pos25 => empty range\n";
      }
   }

   {  // **no random access iterators** -- impl not efficient
      std::list<int> coll;
      // insert elements from 20 to 40
      for (int i=20; i<=40; ++i) { coll.push_back(i); }
      cout << "list: "; for (const auto& e : coll) { cout << e << " "; } cout << "\n";

      // check if pos25 or pos35 comes first
      /*
          pos25: search for 25 in [begin,end)
          pos35: search for 35 in [begin,pos25)
            if it's found
               => I have valid range and I know ordering
            if it's not found
               => pos35: search for 35 in [pos25,end)

      */
      auto pos25 = std::find(coll.begin(), coll.end(), 25);
      auto pos35 = std::find(coll.begin(), pos25, 35);
      //auto it = std::find(coll.end(), coll.end(), 35);
      //cout << "it == coll.end(): " << (it == coll.end()) << "\n";
      //cout << (uint64_t)(&(*it)) << "," << (uint64_t)(&(*coll.end())) << "\n";
      //exit(1);
      if (pos25 != coll.end() && pos35 != pos25) {
         // pos35 before pos25 => only [pos35,pos25) is valid (and non-empty)
         cout << "only [pos35,pos25) is valid (and non-empty)\n";
      }
      else {
         pos35 = std::find(pos25, coll.end(), 35); // actually want: std::find(pos25+1, coll.end(), 35);
         if (pos35 != coll.end()) {
            // pos25 before pos35 => only [pos25, pos35) is valid (and non-empty)
            //    pos35==pos25 is not possible, as *pos25==25 and *pos35==35
            cout << "only [pos25,pos35) is valid (and non-empty)\n";
         }
         else {
            /*
               got here by
               - either
                  'pos25 == coll.end()' => 'pos35 == coll.end()'  // 25 not found, 35 isn't searched for, as 'pos35 = std::find(coll.end(), coll.end(), 35);'
                  => 25 not found (35 is or isn't in coll, we don't know as we didn't check)
                  => (25 not found && 35 not found) || (25 not found && 35 found)
               - or
                  'pos25 != coll.end()' && 'pos35 == pos25'       // 25 found, 35 not found in [begin,pos25)
                     'pos35 == coll.end()'                        // 35 not found in [pos25,end) => 35 is not in coll
                     => 25 found && 35 not found
               
               => 25 and/or 35 not found
            */
         }
      }
   }

   {  // **no random access iterators** -- my idea to clearer structure the above
      std::list<int> coll;
      for (int i=20; i<=40; ++i) { coll.push_back(i); }
      cout << "list: "; for (const auto& e : coll) { cout << e << " "; } cout << "\n";

      auto pos25 = std::find(coll.begin(), coll.end(), 25);
      if (pos25 == coll.end()) {
         cout << "25 not found - not looking for 35\n";
      }
      else {
         cout << "25 found - looking for pos of 35 relative to 25\n";
         auto pos35 = std::find(coll.begin(), pos25, 35);
         if (pos35 != pos25) {
            cout << "35 found in [begin,pos25) => only [pos35,pos25) is valid (and non-empty)\n";
         }
         else {
            pos35 = std::find(pos25, coll.end(), 35);
            if (pos35 != coll.end()) {
               // pos25 before or at pos35 => only [pos25, pos35) is valid (could be empty! if pos35==pos25)
               cout << "only [pos25,pos35) is valid (could be empty!)\n";
            }
            else {
               cout << "35 is not in coll\n";
            }
         }
      }
   }

   {  // **no random access iterators** -- better impl
      /*
         - first, find 1st element that has either value 25 or 35
      */
      std::list<int> coll;
      for (int i=20; i<=40; ++i) { coll.push_back(i); }
      cout << "list: "; for (const auto& e : coll) { cout << e << " "; } cout << "\n";

      auto pos = std::find_if(coll.begin(), coll.end(),
         [](int i){ return (i==25 || i==35); });
      if (pos == coll.end()) {
         cout << "neither 25 nor 35 was found\n";
      }
      else if (*pos==25){
         cout << "25 comes first\n";
         auto pos25 = pos;
         auto pos35 = find(++pos, coll.end(), 35);
         (void) pos25, (void) pos35;
      }
      else {
         // *pos==35
         cout << "35 comes first\n";
         auto pos35 = pos;
         auto pos25 = find(++pos, coll.end(), 25);
         (void) pos25, (void) pos35;
      }
   }
}

void testing_handling_multiple_ranges()
{
   {
      std::list<int> coll1 = {1,2,3,4,5,6,7,8,9};
      std::vector<int> coll2;
      //std::copy(coll1.cbegin(), coll1.cend(), coll2.begin()); // this will be RUNTIME ERROR!!!
      // either 1: make sure destination is large enough or 2: use insert iterators

      coll2.resize(coll1.size());
      std::copy(coll1.cbegin(), coll1.cend(), coll2.begin());
      cout << "coll2 after copy 1: "; for (const auto& e : coll2) { cout << e << " "; } cout << "\n";
      coll2.clear();
      cout << "coll2 after clear:  "; for (const auto& e : coll2) { cout << e << " "; } cout << "\n";

      std::vector<int> coll3(coll1.size());
      std::copy(coll1.cbegin(), coll1.cend(), coll3.begin());
      cout << "coll3 after copy:   "; for (const auto& e : coll3) { cout << e << " "; } cout << "\n";

      std::copy(coll1.cbegin(), coll1.cend(), std::back_inserter(coll2));
      cout << "coll2 after copy 2: "; for (const auto& e : coll2) { cout << e << " "; } cout << "\n";
   }
}

void testing_remove_erase()
{
   {
      std::list<int> coll;
      for (int i=1; i<=6; ++i) {
         coll.push_front(i);
         coll.push_back(i);
      }
      print_elements(coll, "before: ");
      auto end = std::remove(coll.begin(), coll.end(), 3); // remove all elements with value 3
      print_elements(coll, "after:  ");
      
      cout << "only printing up to new logical end: ";
      std::copy(coll.begin(), end, std::ostream_iterator<int>(cout," "));
      cout << "\n";

      cout << "number of removed elements: " << std::distance(end, coll.end()) << "\n";

      coll.erase(end, coll.end());
      print_elements(coll, "after erase: ");
   }
   {
      // same as above, but good performance, as it's using member functions, which is much more efficient for lists
      std::list<int> coll;
      for (int i=1; i<=6; ++i) {
         coll.push_front(i);
         coll.push_back(i);
      }
      print_elements(coll, "before:            ");
      coll.remove(3); // much better than: 'coll.erase(remove(coll.begin(), coll.end(), 3), coll.end());'
      print_elements(coll, "after real remove: ");
   }
   {
      std::set<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      print_elements(coll, "set before removing: ");
      int n_removed = coll.erase(3); // remove all elements with value 3
      cout << "number of removed elements: " << n_removed << "\n";
      print_elements(coll, "set before removing: ");
   }
}

void testing_transform()
{
   {
      std::vector<int> v = { 1, 2, 3, 5, 7, 11, 13, 17, 19 };
      print_elements(v);
      std::transform(v.cbegin(), v.cend(),   // src
                     v.begin(),              // dst
                     std::negate<int>());    // op
      print_elements(v);
      std::transform(v.cbegin(), v.cend(),      // 1st src
                     v.cbegin(),                // 2nd src
                     v.begin(),                 // dst
                     std::multiplies<int>());   // op
      print_elements(v);
   }
   print_hline();
   {
      std::set<int, std::greater<int>> coll1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::deque<int> coll2;
      print_elements(coll1);

      // transform all elements into coll2 by multiplying them with 10
      std::transform(coll1.cbegin(),coll1.cend(),  // src
                     std::back_inserter(coll2),    // dst
                     std::bind(std::multiplies<int>(), std::placeholders::_1, 10));  // op
      print_elements(coll2);

      // replace value equal to 70 with 42
      std::replace_if(coll2.begin(),coll2.end(),
                      std::bind(std::equal_to<int>(), std::placeholders::_1, 70),
                      42);
      print_elements(coll2);

      // remove all elements with values between 50 and 80
      coll2.erase(std::remove_if(coll2.begin(), coll2.end(),
                  // speciﬁes the unary predicate 'x>=50 && x<=80'
                  std::bind(std::logical_and<bool>(),
                     std::bind(std::greater_equal<int>(), std::placeholders::_1, 50),
                     std::bind(std::less_equal<int>(), std::placeholders::_1, 80))),
                  coll2.end());
      print_elements(coll2);
   }
   print_hline();
   {
      // the same as above, but using lambdas instead of bind
      std::set<int, std::greater<int>> coll1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::deque<int> coll2;
      print_elements(coll1);

      std::transform(coll1.cbegin(),coll1.cend(),  // src
                     std::back_inserter(coll2),    // dst
                     [](int i){ return i*10;});  // op
      print_elements(coll2);

      std::replace_if(coll2.begin(),coll2.end(),
                     [](int i){return i==70;},
                     42);
      print_elements(coll2);

      coll2.erase( std::remove_if(coll2.begin(), coll2.end(), [](int i){return i>=50 && i<=80;}), coll2.end() );
      print_elements(coll2);
   }
}

void testing_list_splice()
{
   std::list<int> list1, list2;
   for (int i=0; i<6; ++i) {
      list1.push_back(i);
      list2.push_front(i);
   }
   print_elements(list1, "list1: ");
   print_elements(list2, "list2: ");
   // insert all elements of list1 before 1st element with value 3 of list2
   // find() returns an iterator to the 1st element with value 3
   list2.splice(std::find(list2.begin(), list2.end(), 3), list1);
   print_elements(list1, "list1: ");
   print_elements(list2, "list2: ");
   // move 1st element of list2 to the end
   list2.splice(list2.end(), list2, list2.begin());
   print_elements(list1, "list1: ");
   print_elements(list2, "list2: ");
   // sort list2, assign to list1 and remove duplicates
   list2.sort();
   print_elements(list2, "list2 (after sorting):             ");
   list1 = list2;
   list2.unique();
   print_elements(list2, "list2 (after removing duplicates): ");

   // merge both sorted lists into the 1st list
   print_elements(list1, "list1 (before merging): ");
   print_elements(list2, "list2 (before merging): ");
   list1.merge(list2);
   print_elements(list1, "list1 (after merging):  ");
   print_elements(list2, "list2 (after merging):  ");
}

void testing_forward_list()
{
   {
      std::forward_list<int> l;
      for (int i=1; i<=10; ++i) {
         l.push_front(i);
      }
      print_elements(l, "fw_list: ");
      cout << "fw_list.size(): " << std::distance(l.begin(), l.end()) << " (careful: size() is O(N)!!!)\n";
   }
   {
      std::forward_list<int> l = {1,2,3};
      print_elements(l, "fw_list (before insert): ");
      // insert 77, 88 and 99 at the beginning
      l.insert_after(l.before_begin(), {77,88,99});
      print_elements(l, "fw_list (after insert):  ");
   }
   {
      std::forward_list<int> l = {1,2,3,4,55,66};
      print_elements(l, "fw_list (initially):           ");
      // find the position before the 1st even element
      auto pos_before = l.before_begin();
      for (auto pos=l.begin(); pos != l.end(); ++pos, ++pos_before) {
         if (*pos%2==0) {
            break; // element found
         }
      }
      // and insert a new element in front of the 1st even element
      l.insert_after(pos_before, 42);
      print_elements(l, "fw_list (after insert-after):  ");
   }
   {
      // same as above, using std::next
      std::forward_list<int> l = {1,2,3,4,55,66};
      print_elements(l, "fw_list (initially):           ");
      auto pos_before = l.before_begin();
      for (; std::next(pos_before) != l.end(); ++pos_before) {
         if (*std::next(pos_before)%2==0) {
            break; // element found
         }
      }
      l.insert_after(pos_before, 42);
      print_elements(l, "fw_list (after insert-after):  ");
   }
}

void testing_set()
{
   {
      std::set<int> s = {1,2,3};
      
      auto[pos, wasInserted] = s.insert(4);
      if (wasInserted) {
         cout << "successfully inserted 4 in set\n";
      } else {
         cout << "4 was already in set!\n";
      }

      std::tie(pos, wasInserted) = s.insert(1);
      if (wasInserted) {
         cout << "successfully inserted 1 in set\n";
      } else {
         cout << "1 was already in set!\n";
      }
      print_elements(s);

      // remove all elements up to element with value 3
      s.erase (s.begin(), s.find(3));
      print_elements(s);
   }
   {
      std::set<int> s = {1,2,3};
      decltype(s)::iterator pos;
      bool wasInserted;
      
      std::tie(pos, wasInserted) = s.insert(4);
      if (wasInserted) {
         cout << "successfully inserted 4 in set\n";
      } else {
         cout << "4 was already in set!\n";
      }

      std::tie(pos, wasInserted) = s.insert(1);
      if (wasInserted) {
         cout << "successfully inserted 1 in set\n";
      } else {
         cout << "1 was already in set!\n";
      }
      print_elements(s);
   }
}

void testing_map()
{
   {
      std::map<std::string, int> m = {
         {"1st key", 1},
         {"2nd key", 2},
         {"another key", 3}
      };
      auto print_map = [](decltype(m)& map) {
         for (const auto& [k,v] : map) {
            cout << k << " -> "<<  v << "\n";
         }
         cout << "\n";
      };
      print_map(m);
      m["some new key"] = 42;
      print_map(m);

      {
         auto pos = std::find_if(m.begin(), m.end(),
            [](decltype(m)::value_type& p){ return p.second==3; }); // std::pair<std::string,int>&
         if (pos == m.end()) {
            cout << "3 was not found\n";
         } else {
            cout << "3 was found - its key is '" << pos->first << "'\n";
         }
      }
      {
         decltype(m)::iterator pos;
         for (pos = m.begin(); pos != m.end(); ++pos) {
            if (pos->second==3) {
               cout << "3 was found - its key is '" << pos->first << "'\n";
            }
         }
      }

      {
         // change the key of an element in a map
         decltype(m)::key_type old_key = "2nd key";
         decltype(m)::key_type new_key = "2nd key NEW";
         
         cout << "\nbefore changing key of element:\n";
         print_map(m);
         
         auto pos = m.find(old_key);
         if (pos != m.end()) {
            m.insert({new_key,pos->second});
            m.erase(pos);
         } else {
            cout << "key not found!\n";
         }

         cout << "after changing key of element:\n";
         print_map(m);
      }
      {
         decltype(m)::key_type old_key = "another key";
         decltype(m)::key_type new_key = "+_+";
         
         m[new_key] = m[old_key];   // insert new element with value of old element
         m.erase(old_key);          // remove old element

         cout << "after changing key of element:\n";
         print_map(m);
      }
   }
}

// belongs to 'testing_unordered_container_custom_hash'
struct Customer {
   std::string first_name;
   std::string last_name;
   long no;

   friend std::ostream& operator<<(std::ostream& os, const Customer& c) {
      // p.811
      std::ostringstream oss_no_width;
      oss_no_width.copyfmt(os);
      oss_no_width.width(0);
      oss_no_width << "|" << c.first_name << "," << c.last_name << "," << c.no << "|";
      os << oss_no_width.str();
      return os;
      //return os << "|" << c.first_name << "," << c.last_name << "," << c.no << "|";
   }
};
// own equivalence criterion (p.366: otherwise can't have 'std::unordered_map<Customer, std::string, CustomerHash_better> customer_map;')
bool operator==(const Customer& c1, const Customer& c2) { return c1.no == c2.no; }
// naive hash function
struct CustomerHash_naive {
   std::size_t operator() (const Customer& c) const {
      return std::hash<std::string>()(c.first_name) +
             std::hash<std::string>()(c.last_name) +
             std::hash<long>()(c.no);
   }
};
// better approach
template <typename T>
inline void hash_combine(std::size_t& seed, const T& val) {
   seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}
template <typename T>
inline void hash_val(std::size_t& seed, const T& val) {
   hash_combine(seed,val);
}
template <typename T, typename... Types>
inline void hash_val(std::size_t& seed, const T& val, const Types&... args) {
   hash_combine(seed,val);
   hash_val(seed,args...);
}
template <typename... Types>
inline std::size_t hash_val(const Types&... args) {
   std::size_t seed = 0;
   hash_val(seed,args...);
   return seed;
}
struct CustomerHash_better {
   std::size_t operator() (const Customer& c) const {
      return hash_val(c.first_name,c.last_name,c.no);
   }
};

// helper for 'printHashTableState'
enum class UNORDERED_T { //UNORDERED_CONTAINER_TYPE {
   SET,
   MAP
};
// using the bucket interface
template <typename T, UNORDERED_T cont_type = UNORDERED_T::MAP>
void printHashTableState(const T& cont)
{
   cout << "size:            " << cont.size() << "\n";
   cout << "num of buckets:  " << cont.bucket_count() << "\n";
   cout << "load factor:     " << cont.load_factor() << "\n";
   cout << "max load factor: " << cont.max_load_factor() << "\n";

   if (typeid(typename std::iterator_traits<typename T::iterator>::iterator_category) ==
       typeid(std::bidirectional_iterator_tag))
   {
      cout << "chaining style:  doubly-linked\n";
   } else {
      cout << "chaining style:  singly-linked\n";
   }

   cout << "data:\n";
   for (size_t idx = 0; idx != cont.bucket_count(); ++idx) {
      cout << "  b[" << std::setw(2) << idx << "]: ";

      for (auto pos = cont.begin(idx); pos != cont.end(idx); ++pos) {
         // pos is 'typename T::local_iterator&'
         // TODO: check if we have an unordered_*map or an unordered_*set
         //       if we have T::Key => it's an unordered_*map
         /*if (typeid(typename T::key_type) == typeid(typename T::value_type)) {
            cout << "IT'S A SET!\n";

            //print_bucket_content_set(pos);
         } else {
            cout << "IT'S A MAP!\n";

            //print_bucket_content_map(pos);
         }*/

         // p.397
         if constexpr (std::is_const<typename T::value_type>::value) {
            cout << "[unordered] [multi]set\n";
         } else {
            cout << "[unordered] [multi]map\n";
         }

         if constexpr (cont_type == UNORDERED_T::SET) {
            // T is a set
            cout << *pos << " ";
         } else {
            // T is a map
            std::string v_cpy = pos->second;
            std::replace_if(v_cpy.begin(), v_cpy.end(), [](char c){return c=='\n';}, '#');
            //std::replace(v_cpy.begin(), v_cpy.end(), '\n', '#'); //TODO: try this (p.571), to keep old str: use replace_copy (p.573)
            cout << pos->first << " -> '" << v_cpy << "' ";
         }

      }

      cout << "\n";
   }
}
//template<UNORDERED_T cont_type = UNORDERED_T::MAP, typename T>
template<UNORDERED_T cont_type, typename T>
void testing_sth(const T& cont)
{
   if constexpr (cont_type == UNORDERED_T::SET) {
      for (const auto& e : cont)
         cout << e << " ";
   }
   else {
      for (auto& [k,v] : cont)
         cout << k << "->" << v << " ";
   }
   cout << "\n";
}
template<typename T>
void testing_sth_2(const T& cont)
{
   // 397
   //if constexpr (std::is_const<typename T::value_type>::value) {
   if constexpr (!std::is_object<typename T::value_type::first>::value) {
   //if constexpr (std::is_integral<typename T::value_type>::value) {
   //if constexpr (typename T::value_type == typename T::key_type) {
      cout << "[unordered] [multi]set\n";
      for (const auto& e : cont) {
         cout << e << " ";
      }
   } else {
      cout << "[unordered] [multi]map\n";
      for (auto& [k,v] : cont) {
         cout << k << "->" << v << " ";
      }
   }
   cout << "\n";
}
void testing_unordered_container_custom_hash()
{
   Customer c1 = {"Max", "Mustermann", 42};
   Customer c2 = {"Bob", "Smith", 1};
   Customer c3 = {"Alice", "Smith", 2};
   Customer c4 = {"Alice", "Smitt", 2};
   
   // save format, as std::left applies permanently
   //std::ostringstream s; s.copyfmt(cout);
   auto old_flags = cout.flags();

   {
      cout << "naive hash function:\n";
      {
         cout << "c1: " << std::setw(20) << std::left << c1 << " -- hash: " << CustomerHash_naive()(c1) << "\n";
         cout << "c2: " << std::setw(20) << std::left << c2 << " -- hash: " << CustomerHash_naive()(c2) << "\n";
         cout << "c3: " << std::setw(20) << std::left << c3 << " -- hash: " << CustomerHash_naive()(c3) << "\n";
         cout << "c4: " << std::setw(20) << std::left << c4 << " -- hash: " << CustomerHash_naive()(c4) << "\n";
      }
      cout << "\n";
      {
         CustomerHash_naive hash_func;
         cout << "c1: " << std::setw(20) << std::left << c1 << " -- hash: " << hash_func(c1) << "\n";
         cout << "c2: " << std::setw(20) << std::left << c2 << " -- hash: " << hash_func(c2) << "\n";
         cout << "c3: " << std::setw(20) << std::left << c3 << " -- hash: " << hash_func(c3) << "\n";
         cout << "c4: " << std::setw(20) << std::left << c4 << " -- hash: " << hash_func(c4) << "\n";
      }
   }
   {
      cout << "better hash function:\n";
      {
         cout << "c1: " << std::setw(20) << std::left << c1 << " -- hash: " << CustomerHash_better()(c1) << "\n";
         cout << "c2: " << std::setw(20) << std::left << c2 << " -- hash: " << CustomerHash_better()(c2) << "\n";
         cout << "c3: " << std::setw(20) << std::left << c3 << " -- hash: " << CustomerHash_better()(c3) << "\n";
         cout << "c4: " << std::setw(20) << std::left << c4 << " -- hash: " << CustomerHash_better()(c4) << "\n";
      }
   }

   // restore format
   //cout.copyfmt(s);
   cout.flags(old_flags);

   {
      std::unordered_map<Customer, std::string, CustomerHash_better> customer_map;
      customer_map[c1] += "> customer added to map\n";
      customer_map[c1] += "> customer bought item xyz for ...€\n";
      customer_map.insert({{"fname1","lname1",11},""});
      customer_map.emplace(Customer{"fname2","lname2",22}, "");
      
      cout << "\ncustomer_map:\n";
      for (const auto& [k,v] : customer_map) {
         cout << "customer: " << k << "\n" << v << "\n";
         
         std::string line;
         std::stringstream ss(v);
         int i = 1;
         while (getline(ss,line)) {
            cout << "  " << i++ << ": " << line.substr(2) << "\n";
         }
      }
      
      printHashTableState(customer_map);
      testing_sth<UNORDERED_T::MAP>(customer_map);
      //testing_sth_2(customer_map);
   }

   {
      std::unordered_set<Customer, CustomerHash_better> customer_set;
      customer_set.insert(c1);
      customer_set.insert({"fname1","lname1",11});
      customer_set.emplace(Customer{"fname2","lname2",22});
      
      cout << "\ncustomer_set:\n";
      for (const auto& c : customer_set) {
         cout << "customer: " << c << "\n";
      }
      
      printHashTableState<decltype(customer_set),UNORDERED_T::SET>(customer_set);
      /*
         TODO: this is ugly! better interface by using std::enable_if, std::conditional?  
            template <bool Q=Const> typename std::conditional<Q, const int*, int*>::type operator->() const;
      */
      testing_sth<UNORDERED_T::SET>(customer_set);
      //testing_sth_2(customer_set);
   }

   {
      std::unordered_set<int> intset = { 1,2,3,5,7,11,13,17,19 };
      printHashTableState<decltype(intset),UNORDERED_T::SET>(intset);
      // insert some additional values (might cause rehashing)
      intset.insert({-7,17,33,4});
      printHashTableState<decltype(intset),UNORDERED_T::SET>(intset);
   }
   {
      std::unordered_set<int> intset;
      intset.reserve(9);
      intset.max_load_factor(0.7);
      intset = { 1,2,3,5,7,11,13,17,19 };
      printHashTableState<decltype(intset),UNORDERED_T::SET>(intset);
      // insert some additional values (might cause rehashing)
      intset.insert({-7,17,33,4});
      printHashTableState<decltype(intset),UNORDERED_T::SET>(intset);
   }

   {
      std::unordered_multimap<std::string,std::string> dict = {
            {"day","Tag"},
            {"strange","fremd"},
            {"car","Auto"},
            {"smart","elegant"},
            {"trait","Merkmal"},
            {"strange","seltsam"}
         };
      printHashTableState(dict);
      // insert some additional values (might cause rehashing)
      dict.insert({
         {"smart","raffiniert"},
         {"smart","klug"},
         {"clever","raffiniert"}
      });
      printHashTableState(dict);
      // modify maximum load factor (might cause rehashing)
      dict.max_load_factor(0.7);
      printHashTableState(dict);
   }
}

// for 'testing_container_reference_semantics' (p.388)
struct Item {
   std::string name;
   float price;
   
   Item(const std::string& n, float p=0.0)
      : name(n), price(p) {}
};

template<typename T>
void printItems(const std::string& msg, const T& container) {
   cout << msg << "\n";
   for (const auto& e : container) {
      cout << " " << e->name << ": " << e->price << "\n";
   }
}
void testing_container_reference_semantics()
{
   {
      typedef std::shared_ptr<Item> ItemPtr;
      std::set<ItemPtr> allItems;
      std::deque<ItemPtr> bestsellers;

      bestsellers = {
         std::make_shared<Item>("Kong Yize",20.10),
         std::make_shared<Item>("A Midsummer Night's Dream",14.99),
         std::make_shared<Item>("A Maltese Falcon",9.88)
      };
      allItems = {
         std::make_shared<Item>("Water",0.44),
         std::make_shared<Item>("Pizza",2.22)
      };
      allItems.insert(bestsellers.begin(), bestsellers.end());
      printItems("bestsellers:", bestsellers);
      printItems("all items:", allItems);
      cout << "\n";

      std::for_each(bestsellers.begin(), bestsellers.end(),
         [](ItemPtr& e){e->price *= 2.0;});
      printItems("bestsellers:", bestsellers);
      printItems("all items:", allItems);
      cout << "\n";

      // replace 2nd bestseller by first item with name "Pizza"
      cout << "before: use_count=" << bestsellers[1].use_count() << " (" << bestsellers[1]->name << ")\n";
      cout << "before: use_count=" << (*std::find_if(allItems.begin(), allItems.end(),
         [](ItemPtr e){return e->name == "Pizza";})).use_count() << " (Pizza)\n";
      bestsellers[1] = *std::find_if(allItems.begin(), allItems.end(),
         [](ItemPtr e){return e->name == "Pizza";});
      cout << "after:  use_count=" << (*std::find_if(allItems.begin(), allItems.end(),
         [](ItemPtr e){return e->name == "A Midsummer Night's Dream";})).use_count() << " (A Midsummer Night's Dream)\n";
      cout << "after:  use_count=" << (*std::find_if(allItems.begin(), allItems.end(),
         [](ItemPtr e){return e->name == "Pizza";})).use_count() << " (Pizza)\n";
      printItems("bestsellers:", bestsellers);
      printItems("all items:", allItems);
      cout << "\n";

      bestsellers[0]->price = 99.99;
      printItems("bestsellers:", bestsellers);
      printItems("all items:", allItems);
      cout << "\n";
   }
   {
      std::vector<std::reference_wrapper<Item>> books;

      Item item1("Faust",12.99);
      books.push_back(item1);

      for (const auto& b : books) {
         cout << b.get().name << ": " << b.get().price << "\n";
      }
      for (const Item& b : books) {
         cout << b.name << ": " << b.price << "\n";
      }
      
      item1.price = 9.99;
      cout << books[0].get().price << "\n";

      for (const Item& b : books) {
         cout << b.name << ": " << b.price << "\n";
      }
   }
}
// testing sort objects according to multiple different sorting criteria (p.394)
void testing_multiple_sorting_criteria()
{
   // assume each customer no. is unique, but first_name and last_name duplicates will occur,
   // even same first_name AND last_name would occur multiple times
   // => shouldn't use set then! but maybe multiset?
   // TODO: what about relative order, when last_name/first_name matches, sort by other name, then by no?
   Customer c1  = {"Max",  "Mustermann", 42};
   Customer c2  = {"Bob",  "Smith",       1};
   Customer c3  = {"Anna", "Black",       2};
   Customer c4  = {"Jack", "Smith",       5};
   Customer c5  = {"Jane", "Doe",        19};
   Customer c6  = {"Zulu", "Akebe",       7};
   Customer c7  = {"Greg", "Doherty",    33};

   Customer c8  = {"Paul", "Walker",      8};
   Customer c10 = {"Paul", "Potz",       10};
   Customer c9  = {"Peter", "Pan",        9};
   Customer c11 = {"Ellen", "Pan",       11};
   Customer c12 = {"Ellen", "Pan",       22};

   std::vector<std::reference_wrapper<Customer>> customers = { c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12 };

   cout << "customers database:\n";
   for (size_t i = 0; i < customers.size(); ++i) {
      cout << "  c" << i+1 << ": " << customers[i] << "\n";
   }

   struct CustomerSortCriterion_by_firstName {
      bool operator() (const Customer* c1, const Customer* c2) const {
         return c1->first_name < c2->first_name;
      }
   };
   struct CustomerSortCriterion_by_lastName {
      bool operator() (const Customer* c1, const Customer* c2) const {
         return c1->last_name < c2->last_name;
      }
   };
   struct CustomerSortCriterion_by_customerNo {
      bool operator() (const Customer* c1, const Customer* c2) const {
         return c1->no < c2->no;
      }
   };
   
   std::multiset<Customer*,CustomerSortCriterion_by_firstName> customers_sorted_by_firstName;
   for (const auto& c : customers) { customers_sorted_by_firstName.insert(&c.get()); }   
   cout << "customers sorted by first name:\n";
   for (const auto& c : customers_sorted_by_firstName) {
      cout << "  " << c->first_name << "," << c->last_name << "," << c->no << "\n";
   }

   std::multiset<Customer*,CustomerSortCriterion_by_lastName> customers_sorted_by_lastName;
   for (const auto& c : customers) { customers_sorted_by_lastName.insert(&c.get()); }   
   cout << "customers sorted by last name:\n";
   for (const auto& c : customers_sorted_by_lastName) {
      //cout << " " << c->last_name << "," << c->first_name << "," << c->no << "\n";
      cout << "  " << c->first_name << "," << c->last_name << "," << c->no << "\n";
   }

   std::multiset<Customer*,CustomerSortCriterion_by_customerNo> customers_sorted_by_customerNo;
   for (const auto& c : customers) { customers_sorted_by_customerNo.insert(&c.get()); }   
   cout << "customers sorted by customer no.:\n";
   for (const auto& c : customers_sorted_by_customerNo) {
      cout << "  " << c->no << "," << c->first_name << "," << c->last_name << "\n";
   }
}

void testing_processing_adjacent_elements()
{
   // p.437
   //int arr[] = { 1,1,1,2,2,3,4,4,4,4,5,6,6 };
   int arr[] = { 1,1,1,2,2,3,4,4,4,4,5,6,6,99 };
   //int arr[] = { 1,1 };
   //int arr[] = { 1,2 };
   //int arr[] = { 1,1,1 };
   //int arr[] = { 1,2,3 };

   int *pos, *pos_ahead, *end;
   pos = pos_ahead = arr;
   const int n_elems = sizeof(arr)/sizeof(arr[0]);
   end = &arr[n_elems];
   
   auto print_arr = [&arr]() {
      cout << "arr (vals): ";
      for (const auto& e : arr) { cout << std::setw(2) << e << " "; }
      cout << "\n";
      
      cout << "arr (idxs): ";
      for (size_t i=0; i < std::size(arr); ++i) { cout << std::setw(2) << i << " "; }
      cout << "\n";
   };

   print_arr();   
   cout << "sizeof(arr)             : " << std::setw(2) << sizeof(arr)    << " bytes\n";
   cout << "size of one elem in arr : " << std::setw(2) << sizeof(arr[0]) << " bytes\n";
   cout << "num of elems in arr     : " << std::setw(2) << n_elems << "\n";
   cout << "num of elems in arr     : " << std::setw(2) << (std::end(arr)-std::begin(arr)) << "\n";
   cout << "initially: arr,pos,pos_ahead,end,std::end(arr):"
        << "\n  " << arr
        << "\n  " << pos
        << "\n  " << pos_ahead
        << "\n  " << end
        << "\n  " << std::end(arr)
        << "\n";

   {
      cout << "–––\n";
      if (pos != end) {
         ++pos_ahead;
         while (pos_ahead != end) {
            cout << " (" << pos << " <-> " << pos_ahead << ") " << (*pos_ahead-*pos) << "\n";
            ++pos;
            ++pos_ahead;
         }
      }
   }
   {
      cout << "–––\n";
      pos = pos_ahead = arr;
      /*
         'indices':
         - adjacent elements will be half-open ranges of indices into arr of the form [a,b)
         - iterate over 'indices' and always consider 2 adjacent elems forming a range to get elems of same value in arr
         - e.g.:
            indices == 0, 3, 5, 6, 10, 11, 13, 14
            => [0,3)
            => [3,5)
            => [5,6)
            => [6,10)
            => [10,11)
            => [11,13)
            => [13,14)
       */
      std::vector<int> indices;
      indices.push_back(0);
      if (pos != end) {
         ++pos_ahead;
         while (pos_ahead != end) {
            // inc pos_ahead until different value is hit -- check 'pos_ahead != end' first before '*pos_ahead'
            while (pos_ahead != end && *pos_ahead==*pos) {
               ++pos_ahead;
            }
            if (pos_ahead == end) {
               if (pos == arr) {
                  cout << "all vals in array are the same!\n";
               }
               cout << "no new val since idx " << (pos-arr) << ", num of identical vals: " << (pos_ahead-pos) << "\n";
               break;
            }
            
            const int new_idx = (pos_ahead-arr);
            cout << "new val @idx " << std::setw(2) << new_idx << ", num of identical vals up to pos_ahead: " << (pos_ahead-pos) << "\n";
            indices.push_back(new_idx);

            pos = pos_ahead;
            ++pos_ahead;
         }

         if (indices.back() == n_elems-1) {
            cout << "1 new val @(last valid idx), num of identical vals: " << 1 << "\n";;
         }
         // add end in any case, so subsequent algo can handle ranges uniformly without last value being special
         indices.push_back(n_elems);

         print_elements(indices, "indices: ");
      }
      
      print_arr();

      for (size_t i=0; i<indices.size()-1; ++i) {
         int a = indices[i];
         int b = indices[i+1];
         //const auto [aa,bb] = std::pair<int,int>(indices[i], indices[i+1]);
         cout << "indices in arr in range [" << std::setw(2) << a << "," << std::setw(2) << b << "): ";
         for (int j=a; j<b; ++j) {
            cout << arr[j] << " ";
         }
         cout << "\n";
      }
   }
   {
      // as very first above, using std::next (p.444)
      cout << "–––\n";
      pos = arr;
      if (pos != end) {
         while (std::next(pos) != end) {
            cout << " (" << pos << " <-> " << std::next(pos) << ") " << (*std::next(pos)-*pos) << "\n";
            ++pos;
         }
      }
   }
}

// for 'testing_shift_left'
// precondition: ForwardIterator NEEDS to have public typedef/using of 'value_type'
template<typename ForwardIterator>
void shift_left(ForwardIterator beg, ForwardIterator end) {
   using value_type = typename ForwardIterator::value_type;
   if (beg != end) {
      // save val of 1st elem
      value_type tmp(*beg);
      // shift following values
      ForwardIterator next = beg;
      ++next;
      // if next == end, i.e. 1 value in range [beg,end), don't need (potentially costly) assignment to itself '*beg = tmp;'
      if (next != end) {
         while (next != end) {
            *beg++ = *next++;
         }
         *beg = tmp;
      }
   }
}
void testing_shift_left()
{
   // p.468
   {
      std::vector<int> v = { 0,1,2,3,4,5 };
      print_elements(v, "v initially:        ");
      shift_left(v.begin(), v.end());
      print_elements(v, "v after shift_left: ");
   }
   {
      std::vector<int> v = { 111 };
      print_elements(v, "v initially:        ");
      shift_left(v.begin(), v.end());
      print_elements(v, "v after shift_left: ");
   }
}

void testing_nth_element()
{
   // nth_element() vs. partial_sort()
   /*
      performance of nth_element() vs. partial_sort()
      when finding the first 2 smallest/largest elems in a sequence ?
      => nth_element() should be better as less work is done here, see p.514-515
   */

   const int N = 16;
   {
      std::vector<int> v(N);
      for (size_t i=0; i<v.size(); ++i) { v[i] = int((static_cast<double>(rand())/RAND_MAX)*100); }
      print_elements(v, "v initially:         ");
      // only want 2 smallest elems in v => n=1
      int n = 1;
      std::nth_element(v.begin(), v.begin()+n, v.end());
      cout << "n=" << n << ", i.e. n+1=" << n+1 << " smallest elems in v: ";
      for (int i=0; i<n+1; ++i) { cout << v[i] << " "; }
      cout << "\n";
      print_elements(v, "v after nth_element: ");
      std::sort(v.begin(), v.end());
      print_elements(v, "v sorted:            ");
   }
   cout << "\n";
   {
      std::vector<int> v(N);
      for (size_t i=0; i<v.size(); ++i) { v[i] = int((static_cast<double>(rand())/RAND_MAX)*100); }
      print_elements(v, "v initially:         ");
      // only want 2 largest elems in v => n=1
      int n = 1;
      std::nth_element(v.begin(), v.begin()+n, v.end(), std::greater<int>());
      cout << "n=" << n << ", i.e. n+1=" << n+1 << " largest elems in v: ";
      for (int i=0; i<n+1; ++i) { cout << v[i] << " "; }
      cout << "\n";
      print_elements(v, "v after nth_element: ");
      std::sort(v.begin(), v.end());
      print_elements(v, "v sorted:            ");
   }
   cout << "\n";
   {
      std::vector<int> v(N);
      for (size_t i=0; i<v.size(); ++i) { v[i] = int((static_cast<double>(rand())/RAND_MAX)*100); }
      print_elements(v, "v initially:          ");
      // only want 2 smallest elems
      int n = 2;
      std::partial_sort(v.begin(), v.begin()+n, v.end());
      cout << "n=" << n << " smallest elems in v: ";
      for (int i=0; i<n; ++i) { cout << v[i] << " "; }
      cout << "\n";
      print_elements(v, "v after partial_sort:  ");
      std::sort(v.begin(), v.end());
      print_elements(v, "v sorted:              ");
   }
}


template <int N>
struct factorial_helper {
   static constexpr long val = N*factorial_helper<N-1>::val;
};
template <>
struct factorial_helper<1> {
   static constexpr long val = 1;
};
template <int N>
struct factorial {
   static constexpr long val = factorial_helper<N>::val;
};
void testing_next_permutation()
{
   const int N_elems = 4;
   std::vector<int> v(N_elems);
   for (size_t i=0; i<v.size(); ++i) { v[i] = int((static_cast<double>(rand())/RAND_MAX)*100); }
   print_elements(v, "v initially: ");
   
   std::sort(v.begin(), v.end());
   print_elements(v, "v sorted:    ");

   const long N_permutations = factorial<N_elems>::val;
   cout << "the following should yield " << N_permutations << " permutations:\n";

   int i = 1;
   while (std::next_permutation(v.begin(), v.end())) {
      printf("permutation %2d/%2ld of v: ", i++, N_permutations);
      print_elements(v);
   }
   cout << "–––\n";
   printf("permutation %2d/%2ld of v: ", i, N_permutations);
   print_elements(v);
}

void testing_shuffle()
{
   const int N = 20;
   std::vector<int> v(N);
   for (size_t i=0; i<v.size(); ++i) { v[i] = int((static_cast<double>(rand())/RAND_MAX)*100); }
   print_elements(v, "v initially:       ");
   std::random_shuffle(v.begin(), v.end());
   print_elements(v, "v after shuffle 1: ");
   std::random_shuffle(v.begin(), v.end());
   print_elements(v, "v after shuffle 2: ");

   std::default_random_engine dre;
   std::shuffle(v.begin(), v.end(), dre);
   print_elements(v, "v after shuffle 3: ");
}

void testing_bitset()
{
   {
      for (const int i : {7,10,42}) {
         cout << "i=" << i << " (decimal hex binary):\n"
              << "  " << i << " "
              << std::hex << std::showbase << i << " "
              << std::dec << std::bitset<16>(i) // std::dec to reset from std::hex
              << "\n";
         cout << "  binary(" << i << "),num set bits: " << std::bitset<16>(i) << "," << std::bitset<16>(i).count() << "\n";
      }
   }
   {
      int i = 12345678;
      std::string s = std::bitset<32>(i).to_string();
      cout << "i=" << i << " as string: " << s << "\n";
   }
   {
      std::string s = "1010111100001001";
      unsigned long num = std::bitset<32>(s).to_ulong();
      cout << "s=\"" << s << "\" as number: " << num << "\n";
   }
}

void testing_string()
{
   {
      // p.656
      std::vector<std::string> strings = {
         "prog.dat",
         "mydir",
         "hello.",
         "oops.tmp",
         "end.dat",
         "some.file.name.multiple.dots",
      };
      print_elements(strings, "strings: ");
      const std::string suffix = "tmp";
      cout << "–––\n";
      for (const auto& s : strings) {
         {
            auto idx = s.find('.'); // rfind to find last '.'
            if (idx == std::string::npos) {
               cout << "|" << s << "| contains NO \'.\'\n";
            } else {
               cout << "|" << s << "| contains \'.\'\n";
               std::string base_name = s.substr(0,idx);
               std::string ext_name = s.substr(idx+1);
               cout << "  base_name,ext_name: " << base_name << "," << ext_name << "\n";
               if (ext_name.empty()) {
                  cout << "  \'.\' is very last character of file_name, no extension present\n";
               } else {
                  cout << "  extension present\n";
                  if (ext_name == suffix) {
                     cout << "  file name already has suffix " << suffix << "\n";
                  } else {
                     cout << "  replacing extension |" << ext_name << "| with |" << suffix <<  "|\n";
                     std::string tmp = s;
                     tmp.replace(idx+1, std::string::npos, suffix);
                     cout << "  orig replaced: " << s << " " << tmp << "\n";
                  }
               }
            }
         }
      }
      cout << "–––\n";
   }

   {
      // p.661
      std::string input = " pots & pans\nI saw a reed  \n  deliver no pets \nnametag on diaper \neno\n";
      std::stringstream ss(input);
      
      std::string line;
      std::string::size_type begIdx, endIdx;
      const char delim = ' '; // const string delims(" \t,.;");
      
      // while (getline(std::cin,line)) {
      while (getline(ss,line)) {
         //cout  << "line: " << line << "\n";
         cout << "  ";

         // search beginning of first word
         begIdx = line.find_first_not_of(delim);
         
         // while beginning of a word is found
         while (begIdx != std::string::npos) {
            
            // search end of word
            endIdx = line.find_first_of(delim, begIdx);
            
            if (endIdx == std::string::npos) {
               // end of word is end of line
               endIdx = line.length();
            }

            // print chars in reverse order
            for (int i=endIdx-1; i>=(int)begIdx; --i) {
               cout << line[i];
            }
            cout << " ";

            // search beginning of next word
            begIdx = line.find_first_not_of(delim, endIdx);
         }
         cout << "\n";
      }
      cout << "–––\n";
   }

   {
      // tokenize using getline(), p.678 (see also p.729)
      // -- similar to boost::split()
      // -- std::vector<std::string> tokenize(const std::string& input, const char token_sep)

      //std::string input = " some text & 42:another token:1234 last token";
      std::string input = " some text & 42:another token:1234 last token:";
      //std::string input = " some text & 42:another token:1234 last token:a";
      //std::string input = " some text & 42:another token :1234 token: : ";
      //std::string input = ":::";
      cout << "input: |" << input << "|\n\n";
      
      std::istringstream iss(input);
      const char token_sep = ':';
      
      std::vector<std::string> token_vec; // result: vector of tokens
      std::string token;
      while (getline(iss,token,token_sep)) {
         cout  << "token: |" << token << "|\n";

         // remove leading and trailing whitespaces
         auto begIdx = token.find_first_not_of(' ');
         //cout << "  begIdx=" << begIdx << "\n";
         if (begIdx == std::string::npos) {
            // only spaces, add empty string
            token_vec.push_back("");
         }
         else {
            auto endIdx = token.find_last_not_of(' ');
            //cout << "  endIdx=" << endIdx << "\n";
            assert(endIdx != std::string::npos);
            cout << "  substr: |" << token.substr(begIdx, endIdx-begIdx+1) << "|\n";
            token_vec.push_back(std::move(token));
         }
      }

      cout << "stream state:                " << std::bitset<16>(iss.rdstate())     << "\n";
      cout << "flags triggering exceptions: " << std::bitset<16>(iss.exceptions())     << "\n";
      cout << "goodbit (no bit set):        " << std::bitset<16>(std::ios::goodbit) << "\n";
      cout << "failbit:                     " << std::bitset<16>(std::ios::failbit) << "\n";
      cout << "badbit:                      " << std::bitset<16>(std::ios::badbit)  << "\n";
      cout << "eofbit:                      " << std::bitset<16>(std::ios::eofbit)  << "\n";

      // check if last  char in input == token_sep, and add "" to token_vec if so
      iss.clear(); // clear eofbit and failbit
      cout << "stream state:         " << std::bitset<16>(iss.rdstate()) << " (after clear)" << "\n";
      iss.seekg(-1,std::ios::end); // seek to last char of stream
      //char last_char = iss.get(); // see p.755, p.768
      char last_char;
      iss >> last_char;
      //iss.seekg(0);
      cout << "last_char: |" << last_char << "|\n";
      if (last_char == token_sep) {
         token_vec.push_back("");
      }

      cout << "found " << token_vec.size() << " tokens (" << token_vec.size()-1 << " token_sep \'" << token_sep << "\'):\n";
      for (const auto& t : token_vec) {
         cout  << "  |" << t << "|\n";
      }
      cout << "–––\n";
   }

   {
      std::string s;
      cout << "capacity of empty string: " << s.capacity() << "\n";
      cout << "–––\n";
   }

   {
      // p.686
      std::string s = "The C++ Standard Library: A Tutorial and Reference";
      cout << "s: \"" << s << "\"\n";
      {
         for (std::string query : {"tutorial", "Tutorial"}) {
            cout << "query: \"" << query << "\" => ";
            
            auto pos = s.find(query);
            if (pos == std::string::npos) {
               cout << "\"" << query << "\" not found\n";
            } else {
               cout << "\"" << query << "\" found @pos " << pos << "\n";
            }
         }
         cout << "–––\n";
      }
      {
         for (std::string query : {"tutorial", "Tutorial"}) {
            cout << "query (search case-insensitive): \"" << query << "\" => ";
            
            auto pos = std::search(
               s.cbegin(), s.cend(), 
               query.cbegin(), query.cend(),
               [](char a, char b) { return std::toupper(a)==std::toupper(b); }
            );
            if (pos == s.end()) {
               cout << "\"" << query << "\" not found\n";
            } else {
               cout << "\"" << query << "\" found @pos " << (pos-s.cbegin()) << "\n";
            }
         }
         cout << "–––\n";
      }
      {
         std::sort(s.begin(), s.end());
         cout << "sorted:        |" << s << "|\n";
         s.erase(std::unique(s.begin(),s.end()), s.end());
         cout << "no duplicates: |" << s << "|\n";
         cout << "–––\n";
      }
   }
   {
      using T1 = unsigned short;
      using T2 = unsigned int;
      using T3 = unsigned long;
      cout << "size: " << sizeof(T1) << ", unsigned -1: " << std::bitset<64>((T1) -1) << "\n";
      cout << "size: " << sizeof(T2) << ", unsigned -1: " << std::bitset<64>((T2) -1) << "\n";
      cout << "size: " << sizeof(T3) << ", unsigned -1: " << std::bitset<64>((T3) -1) << "\n";
   }
}

void testing_stream_redirect()
{
   cout << "first row\n";
   
   {
      std::ofstream file("redirect.txt");
      
      /*auto del = [&](std::streambuf* p) { cout.rdbuf(p); };
      std::unique_ptr<std::streambuf, decltype(del)> old_buf(cout.rdbuf(), del);*/

      // save old buffer
      auto orig_buf = cout.rdbuf();
      
      // redirect output for cout in into file
      cout.rdbuf(file.rdbuf());
      
      file << "1st row in file\n";
      cout << "one row for stream\n";
      
      // restore buffer
      cout.rdbuf(orig_buf);
   }
   
   cout << "last row\n";
}

void testing_locales()
{
   /*
      check:
         $ locale -a                      # installed locales on machine
         $ cat /usr/share/i18n/SUPPORTED  # supported locales on machine

         $ ls /var/lib/locales/supported.d/
         $ cat /etc/locale.gen
         $ apt-cache search language-pack
         $ ibus-setup
   */
   std::vector<std::string> locale_names = {
      "C",
      "de_DE.utf8",
      "en_AU.utf8",
      "en_CA.utf8",
      "en_GB.utf8",
      "en_US.utf8",
      "ja_JP.utf8",
      "ko_KR.utf8",
      "POSIX",
      "ru_RU.utf8",
      "zh_CN.utf8",
      "zh_HK.utf8",
      "zh_TW.utf8",
      "zh_SG.utf8"
   };

   cout << "<locale name> : ...\n\n";
   for (const auto& locale_name : locale_names) {
      cout << "  " << std::setw(12) << std::left << locale_name << ": ";
      
      std::locale loc(locale_name);
      cout.imbue(loc);
      
      cout << "true = " << std::use_facet<std::numpunct<char>>(loc).truename() << "\n";

      auto now = std::chrono::system_clock::now();
      auto t = std::chrono::system_clock::to_time_t(now);
      struct tm* nowTM = std::localtime(&t);
      
      #if 0
      cout << "struct tm, size is " << sizeof(struct tm) << ":\n";
      for (int i = 0; i < sizeof(struct tm); ++i) {
         //cout << std::hex << (*((int*)((char*)nowTM+i)) & 0xff) << " ";
         int hex_val = ( ((int) *((char*)nowTM+i) ) & 0xff );
         cout << std::hex << std::setfill('0') << std::setw(2) << hex_val << " ";
      }
      cout << "\n";
      cout << std::dec; // reset
      cout << "secs:  " << ( ((int)*((char*)nowTM+0)) & 0xff ) << "\n";
      cout << "mins:  " << ( ((int)*((char*)nowTM+4)) & 0xff ) << "\n";
      cout << "hours: " << ( ((int)*((char*)nowTM+8)) & 0xff ) << "\n";
      #endif

      const char* format_string = 
         "  date:           %x\n"
         "  time:           %X\n"
         "  weekday:        %A\n"
         "  month:          %B\n"
         "  day date time:  %A %x %I:%M %p\n"; // p.886: weekday, date, hour, minutes, AM/PM
      // cout << std::put_time(nowTM,"  date: %x\n  time: %X\n") << "\n";
      cout << std::put_time(nowTM, format_string);

      const auto& mp = std::use_facet<std::money_put<char>>(loc);
      cout << std::showbase; // to show the currency symbol
      /* p.881: 
         Note that the unit used for the passed monetary value passed to put() is
         Cent in the United States or Eurocent in Europe.
         When passing a long double the fractional part of the value is rounded,
         when passing a string it is truncated.
      */
      cout << "  "; mp.put(cout, false, cout, ' ', 12345.678); cout << "\n";
      //cout << "  "; mp.put(cout, false, cout, ' ', "12345.678"); cout << "\n";
      cout << "  " << std::put_money(12345.678, false) << "\n";
      cout << "  "; mp.put(cout, true,  cout, ' ', 12345.678); cout << "\n";

      const auto& np = std::use_facet<std::num_put<char>>(loc);
      cout << "  "; np.put (cout, cout, ' ', 1234.5678); cout << "\n";
      cout << std::fixed;
      cout << "  "; np.put (cout, cout, ' ', 1231234.5678); cout << "\n";
      cout << "\n";
      cout << std::noshowbase; // reset
   }

   cout.imbue(std::locale("")); // reset
}

std::wstring to_wstring (const std::string& str, const std::locale& loc = std::locale())
{
   std::vector<wchar_t> buf(str.size());
   std::use_facet<std::ctype<wchar_t>>(loc).widen(str.data(), str.data()+str.size(), buf.data());
   return std::wstring(buf.data(),buf.size());
}
std::wstring utf8_to_wstring (const std::string& str)
{
   std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
   return myconv.from_bytes(str);
}
void testing_string_conversions()
{
   // TODO
   /*
   cout << "space sym: \xcb\xbd" << "\n";
   std::string s = "\xce\x91";
   cout << *((uint16_t*)s.data()) << "\n";
   cout << *((uint16_t*)s.data())+1 << "\n";
   cout << "\xce\x91" << "\n";
   for (uint16_t c=0xce91; c<=0xcea9; ++c) { cout << std::string((char*)&c,2).append("\0"); } cout << "\n";
   for (uint16_t c=0xceb1; c<=0xcf89; ++c) { cout << std::string((char*)&c,2).append("\0"); } cout << "\n";
   exit(-1);
   */

   auto string_to_wstring = [](const std::string& str) -> std::wstring {
      std::vector<wchar_t> buf(str.size());
      std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(str.data(), str.data()+str.size(), buf.data());
      return std::wstring(buf.data(), buf.size());
   };

   std::string narrow = "0123456789";
   std::wstring wide = string_to_wstring(narrow);

         cout << "narrow: " << narrow << "\n";
   std::wcout << "wide:   " << wide << "\n" << std::flush;

   for (size_t i = 0; i < narrow.size()*sizeof(decltype(narrow)::value_type); ++i) {
      cout << std::setfill('0') << std::setw(2) << (int)*((char*)narrow.data()+i) << " ";
   }
   cout << "\n";

   for (size_t i = 0; i < wide.size()*sizeof(decltype(wide)::value_type); ++i) {
      cout << std::setfill('0') << std::setw(2) << (int)*((char*)wide.data()+i) << " ";
   }
   cout << "\n";
   cout << std::setfill(' '); // reset
}


template <typename Distribution, typename Engine>
void show_dist(Distribution d, Engine e, const std::string& name)
{
   cout << name << ":\n";
   cout << " min: " << d.min() << "\n";
   cout << " max: " << d.max() << "\n";

   std::map<long,int> value_counter;
   for (size_t i=0; i<20000; ++i) {
      ++value_counter[d(e)];
   }

   const auto [min,max] = std::minmax_element(
      value_counter.begin(), value_counter.end(),
      [](const auto& p1, const auto& p2) { // 'const auto&' instead of 'const std::pair<long,int>&'
         return p1.second < p2.second;
      });
   cout << "min/max count idx: " << min->first << "," << max->first << "\n";
   cout << "min/max count val: " << min->second << "," << max->second << "\n";
   const int val_range = max->second - min->second + 1;
   const int max_N_chars = 40;
   const double counts_per_char = (double)val_range/max_N_chars;
   const int offset = (double)min->second/max_N_chars;
   cout << "val_range: " << val_range << ", max_N_chars: "
        << max_N_chars << ", counts_per_char: " << counts_per_char << "\n";
   /* establish mapping
         0          -> 0
         min-value  -> N_min
         max-value  -> N_max
         s.t. N_max-N_min == desired dynamic range 
   */
   for (const auto& [k,v] : value_counter) {
      cout << std::setw(3) << k << ": " << v << "\n";
   }
   cout << "–––\n";
   cout << std::setw(3) << "~" << ": " << std::string(offset,'>') << std::string(max_N_chars,'-') << "\n";
   for (const auto& [k,v] : value_counter) {
      cout << std::setw(3) << k << ": " << std::string(int((v-min->second)/counts_per_char)+offset, '+') << "\n";
   }
}
void testing_random()
{
   std::default_random_engine dre;
   int a = -10, b = 10;
   std::uniform_int_distribution<int> dist_int(a,b); // uniform in [a,b]
   for (int i=0; i<30; ++i) { cout << dist_int(dre) << " "; } cout << "\n";

   std::uniform_real_distribution<float> dist_real(a,b); // uniform in [a,b)
   for (int i=0; i<10; ++i) { cout << dist_real(dre) << " "; } cout << "\n";

   std::vector<int> v = { 1,2,3,4,5,6,7,8,9 };
   print_elements(v, "initially: ");
   std::shuffle(v.begin(), v.end(), dre);
   print_elements(v, "shuffled:  ");
   std::random_shuffle(v.begin(), v.end());
   print_elements(v, "shuffled:  ");

   std::vector<int> v1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   std::vector<int> v2 = { 8, 7, 5, 6, 2, 4, 9, 3, 1 };
   std::vector<int> v3 = { 3, 9, 2, 4, 7, 6, 1, 5, 8 };
   auto determine_index_mapping = [](std::vector<int>& orig, std::vector<int>& shuffled) {
      const auto N = orig.size();
      for (size_t i=0; i<N; ++i) {
         const auto& current_elem = orig[i];
         for (size_t j=0; j<N; ++j) {
            if (shuffled[j] == current_elem) {
               cout << "index mapping: " << i << " –> " << j << "\n";
               break;
            }
         }
      }
   };
   determine_index_mapping(v1,v2);
   cout << "–––\n";
   determine_index_mapping(v2,v3);
   cout << "–––\n";
   determine_index_mapping(v1,v3);
   cout << "–––\n";

   std::mt19937 engine;
   std::uniform_int_distribution<> dist_uniform(0,10);
   show_dist(dist_uniform, engine, "uniform");
   std::normal_distribution<> dist_normal;
   show_dist(dist_normal, engine, "normal");
   std::exponential_distribution<> dist_exp;
   show_dist(dist_exp, engine, "exp");
   std::cauchy_distribution<> dist_cauchy;
   show_dist(dist_cauchy, engine, "cauchy");
}

int do_work(char c)
{
   std::default_random_engine e(c);
   std::uniform_int_distribution<int> dist(10,1000);
   for (int i=0; i<10; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(dist(e)));
      cout.put(c).flush();
   }
   return c;
}
int f1() { return do_work('.'); }
int f2() { return do_work('+'); }
void testing_async_future()
{
   goto ASYNC_LABEL;
   // !!! need to link with libpthread => '-lpthread' !!!
   {
      cout << "starting f1 in bg and f2 in fg:\n" << std::flush;
      std::future<int> res1 = std::async(f1);   // start f1 (now or later or never)
      int res2 = f2();                          // call f2 synchronously
      int res = res1.get() + res2;              // wait for f1 to finish
      cout << "\n" << "result of f1()+f2(): " << res << "\n";
   }
   cout << "–––\n";
   {
      cout << "starting 2 tasks asynchronously\n";
      auto f1 = std::async([]{ do_work('.'); });
      auto f2 = std::async([]{ do_work('+'); });
      if (f1.wait_for(std::chrono::seconds(0)) != std::future_status::deferred ||
          f2.wait_for(std::chrono::seconds(0)) != std::future_status::deferred)
      {
         while (f1.wait_for(std::chrono::seconds(0)) != std::future_status::ready &&
                f2.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
         {
            ;
            std::this_thread::yield();
         }
      }
      cout.put('\n').flush();
      f1.get();
      f2.get();
      cout << "\n" << "done\n";
   }
   cout << "–––\n";
   {
      cout << "using func-ptr, starting 2 tasks asynchronously, 1 synchronously\n";
      auto f1 = std::async(do_work, '.');
      auto f2 = std::async(do_work, '+');
      do_work('#');
      f1.get();
      f2.get();
      cout << "\n" << "done\n";
      /* !! fishy output !! (reason is 'std::ios::sync_with_stdio(false);' (see also p.845,p.985) => UB)

            using func-ptr, starting 2 tasks asynchronously, 1 synchronously
            #
            done
            –––
            using func-ptr, starting 2 tasks asynchronously, 1 synchronously
            #
            done
            –––
            using func-ptr, starting 2 tasks asynchronously, 1 synchronously
            #+.#+..##+...#+..#+#.++++#+##
            done
            –––
      */
   }
   cout << "–––\n";
   {
      cout << "main ID:     " << std::this_thread::get_id() << "\n";
      cout << "nothread ID: " << std::thread::id() << "\n";
      std::thread::id mainTID = std::this_thread::get_id();
      cout << "main ID:     " << mainTID << "\n";

      std::thread t1(do_work, '.');
      cout << "started fg thread " << t1.get_id() << "\n";
      for (int i=0; i<5; ++i) {
         std::thread t(do_work, 'a'+i);
         cout << "detach started fg thread " << t.get_id() << "\n";
         t.detach();
      }
      cout << "waiting for key press...\n";
      std::cin.get();
      cout << "join fg thread " << t1.get_id() << "\n";
      t1.join();
   }
   cout << "–––\n";
   {
      auto do_work = [](std::promise<std::string>& p) {
         cout << "type and char, followed by ENTER:\n";
         char c = std::cin.get();
         std::string s = std::string("char ") + c + " processed";
         p.set_value(std::move(s));
         //p.set_value_at_thread_exit(std::move(s));
         /* p.972
            "Note that get() blocks until the shared state is ready,
             which is exactly the case when set_value() or set_exception() was performed for the promise.
             It does not mean that the thread setting the promise has ended.
             The thread might still perform other statements,
             such as even store additional outcomes into other promises.

             If you want the shared state to become ready when the thread really ends — to ensure the
             cleanup of thread local objects and other stuff before the result gets processed — you
             have to call set_value_at_thread_exit() instead"
         */
      };

      std::promise<std::string> p;
      std::thread t(do_work, std::ref(p));
      
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));

      std::future<std::string> f(p.get_future());  // can also be created before starting thread t (p.971)
      cout << "result: " << f.get() << "\n";       // blocks until p.set_value was performed
      // if I don't detach t, get crash when I don't call join, even if I use set_value_at_thread_exit() !?! WHY ??
      // I don't think that's a problem with pthread, when not joining - do I have to detach or call join it not detaching ??
      // => p.979: yup, seems I HAVE TO call either detach() or join()....
      t.join();
   }
   ASYNC_LABEL:
   cout << "–––\n" << std::flush;
   {
      cout << "num of threads supported by HW: " << std::thread::hardware_concurrency() << "\n";
      auto compute = [](int x, int y) -> double { return x+y; };
      //std::packaged_task<decltype(compute)> task(compute);
      std::packaged_task<double(int,int)> task(compute);
      std::future<double> f = task.get_future();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      task(100,42);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      auto res = f.get();
      cout << "res of task: " << res << "\n";
   }
   cout << "–––\n";
}

std::mutex mtx;
void print_str(const std::string& s) {
   std::lock_guard<std::mutex> lg(mtx);
   for (const char& c: s) {
      cout.put(c);
   }
   cout << "\n";
}
// polling example
bool readyFlag = false;
std::string thread_data;
void thread1_poll() {
   thread_data = "t1 finished preparing data for t2";
   std::this_thread::sleep_for(std::chrono::milliseconds(2000));
   std::lock_guard<std::mutex> lg(mtx);
   readyFlag = true;
}
void thread2_poll() {
   { // scope for lock
      std::unique_lock<std::mutex> ul(mtx);
      while (!readyFlag) {
         ul.unlock();
         cout.put('.').flush();
         std::this_thread::yield();
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
         ul.lock();
      }
   }
   cout << "\n";
   cout << "This is thread2 with id " << std::this_thread::get_id() << ".\n";
   cout << "Got data: " << thread_data << "\n";
}
std::condition_variable cond_var;
void thread1_cond_var() {
   thread_data = "t1 finished preparing data for t2";
   std::this_thread::sleep_for(std::chrono::milliseconds(2000));
   {
      std::lock_guard<std::mutex> lg(mtx);
      readyFlag = true;
   }
   cond_var.notify_one();
}
void thread2_cond_var() {
   { // scope for lock
      cout << "t2 is waiting for cond var" << std::flush;
      std::unique_lock<std::mutex> ul(mtx);
      //  to deal with spurious wakeups, use either
      //       while (!readyFlag) { cond_var.wait(ul); }
      //  or
      //       cond_var.wait(ul,[]{ return readyFlag;});
      cond_var.wait(ul,[]{ return readyFlag; });
   }
   cout << "\n";
   cout << "cond var signaled - This is thread2 with id " << std::this_thread::get_id() << ".\n";
   cout << "Got data: " << thread_data << "\n";
}
std::condition_variable condVar_canRead;
std::condition_variable condVar_canWrite;
std::mutex mtx_canRead;
std::mutex mtx_canWrite;
bool b_canRead = false;    // initially, cannot read but write
bool b_canWrite = true;    // initially, cannot read but write
const int BUF_SIZE = 100;
int shared_buffer[BUF_SIZE];
int shared_idx = 0;
void thread_producer() {
   // produce 100 items in total, always signal consumer after 10 newly produced items
   int count_produced_items = 0;
   while (count_produced_items < BUF_SIZE)
   {
      // check if I can write
      {
         std::unique_lock<std::mutex> ul(mtx_canWrite);
         condVar_canWrite.wait(ul,[]{ return b_canWrite; });
         b_canWrite = false;
      }
      // write 10 values in shared buffer
      cout << "producer: ";
      for (int end=shared_idx+10; shared_idx<end; ++shared_idx) {
         shared_buffer[shared_idx] = shared_idx*100;
         cout.put('.').flush();
      }
      cout.put('\n').flush();
      count_produced_items += 10;

      // signal consumer that it can read
      {
         std::lock_guard<std::mutex> lg(mtx_canRead);
         b_canRead = true;
      }
      condVar_canRead.notify_one();
   }
}
void thread_consumer() {
   int count_consumed_items = 0;
   while (count_consumed_items < BUF_SIZE)
   {
      // check if I can read
      {
         std::unique_lock<std::mutex> ul(mtx_canRead);
         condVar_canRead.wait(ul,[]{ return b_canRead; });
         b_canRead = false;
      }

      // write 10 values in shared buffer
      cout << "consumer: ";
      for (int i=10; i>0; --i) {
         cout << std::setw(2) << i << ":" << std::setw(4) << shared_buffer[shared_idx-i] << " ";
      }
      count_consumed_items += 10;
      cout.put('\n').flush();

      // signal producer that it can write again
      {
         std::lock_guard<std::mutex> lg(mtx_canWrite);
         b_canWrite = true;
      }
      condVar_canWrite.notify_one();
   }
}
void testing_locking()
{
   {
      auto f1 = std::async(std::launch::async, print_str, "Hello from thread 1");
      auto f2 = std::async(std::launch::async, print_str, "Hello from thread 2");
      print_str("Hello from main thread");
   }
   cout << "–––\n";
   
   {
      // polling example p.997
      cout << "launching 2 threads, one is waiting(polling) for the other to prep sth.\n";
      auto f1 = std::async(std::launch::async, thread2_poll);
      auto f2 = std::async(std::launch::async, thread1_poll);
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(2500));
         std::lock_guard<std::mutex> lg(mtx);
         readyFlag = false;
         cout << "main thread reset flag\n";
      }
   }
   cout << "–––\n";
   
   {
      // signaling by condition variable example p.1003
      cout << "launching 2 threads, one is waiting(on condition var) for the other to prep sth.\n";
      auto f1 = std::async(std::launch::async, thread2_cond_var);
      auto f2 = std::async(std::launch::async, thread1_cond_var);
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(2500));
         std::lock_guard<std::mutex> lg(mtx);
         readyFlag = false;
         cout << "main thread reset flag\n";
      }
   }
   cout << "–––\n";

   {
      cout << "launching 2 threads, producer & consumer, signalling each other with 2 condition vars\n";
      std::thread t1(thread_consumer);
      std::thread t2(thread_producer);
      cout << "main thread would have finished here\n";
      t1.join();
      t2.join();
   }
   cout << "–––\n";

   {
      std::atomic<bool> flag_atomic(false);
      const bool is_lock_free = flag_atomic.is_lock_free();
      if (is_lock_free)
         cout << "native HW support for atomics w/o locking\n";
      else
         cout << "no native HW support for atomcis (have to use locks)\n";
   }
   cout << "–––\n";
}

void testing_offsetof()
{
   // offsetof(T,m)
   struct S {
      int a;
      int b;
      char c;
      char c2;
      float f;
   };
   struct S_wasting_mem {
      int a;
      int b;
      char c;
      float f;
      char c2;
   };
   using u64 = uint64_t;
   cout << "sizeof(S):             " << sizeof(S)               << "\n";
   cout << "sizeof(S_wasting_mem): " << sizeof(S_wasting_mem)   << "\n";
   cout << "offset of a:  " << (u64) &(((struct S*)0)->a)  << "\n";
   cout << "offset of b:  " << (u64) &(((struct S*)0)->b)  << "\n";
   cout << "offset of c:  " << (u64) &(((struct S*)0)->c)  << "\n";
   cout << "offset of c2: " << (u64) &(((struct S*)0)->c2) << "\n";
   cout << "offset of f:  " << (u64) &(((struct S*)0)->f)  << "\n";
   
   S s;
   cout << "S s @addr:    " << (u64) &s                         << "\n";
   cout << "offset of a:  " << (u64) ((char*)&s.a  - (char*)&s) << "\n";
   cout << "offset of b:  " << (u64) ((char*)&s.b  - (char*)&s) << "\n";
   cout << "offset of c:  " << (u64) ((char*)&s.c  - (char*)&s) << "\n";
   cout << "offset of c2: " << (u64) ((char*)&s.c2 - (char*)&s) << "\n";
   cout << "offset of f:  " << (u64) ((char*)&s.f  - (char*)&s) << "\n";

   // these don't take alignment requirements into account !!!
   cout << (u64) ((char*)0) << "\n";
   cout << (u64) ((char*)0+sizeof(S::a)) << "\n";
   cout << (u64) ((char*)0+sizeof(S::a)+sizeof(S::b)) << "\n";
   cout << (u64) ((char*)0+sizeof(S::a)+sizeof(S::b)+sizeof(S::c)) << "\n";
   cout << (u64) ((char*)0+sizeof(S::a)+sizeof(S::b)+sizeof(S::c)+sizeof(S::f)) << "\n";
}


int main(int argc, char* argv[])
{
   std::ios::sync_with_stdio(false);
   goto HERE;


   print_hline();
   {
      cout << "all " << argc << " args in argv:\n";
      for (int i = 0; i < argc; ++i) {
         std::string arg = argv[i]; // see p.657
         cout << "  |" << argv[i] << "|\n";
      }
      cout << "__cplusplus: " << __cplusplus << "\n";
      cout << nullptr << "," << (uint64_t)nullptr << "," << (void*)0 << "\n";

      char* p = nullptr;
      cout << "p initially:    " << (uint64_t)p << "\n";
      p  = new char[20];
      cout << "p after new:    " << (uint64_t)p << "\n";
      delete[] p;
      cout << "p after delete: " << (uint64_t)p << "\n"; // delete doesn't set ptr to NULL...

      const char* env_var = "MK_DEF"; // make && MK_DEF=1 ./book_1
      char* envp = getenv(env_var);
      if (envp) cout << "environment variable '" << env_var << "' is " << envp << "\n";
      else cout << "environment variable '" << env_var << "' is not set" << "\n";

      envp = getenv("LANG");
      if (envp) cout << "environment variable 'LANG' is " << envp << "\n";
      else cout << "environment variable 'LANG' is not set\n";
   }
   print_hline();

   testing_offsetof();
   print_hline();

   {
      auto print_from_initializer_list = [](std::initializer_list<int> l) -> void {
         for (const auto& e : l) {
            cout << e << ", ";
         }
         cout << "\n";
      };
      cout << "type of lambda: " << typeid(decltype(print_from_initializer_list)).name() << "\n";
      print_from_initializer_list({1,2,3});
   }
   print_hline();

   {
      // useful for initializing a matrix
      std::vector<std::vector<int>> mat;
      auto init_from_initializer_list = [&mat](std::initializer_list<std::initializer_list<int>> ll) -> void {
         mat.clear(); // does this call dtor of all inner vectors => yes, it does!!!
         for (const auto& l : ll) {
            std::vector<int> v = l; // can assign initializer_list to vector
            mat.push_back(l);
            for (const auto& e : l) {
               cout << e << ", ";
            }
            cout << "\n";
         }
         cout << "\n";
      };
      {
         init_from_initializer_list({{1,2,3,4}, {11,22,33,44}, {111,222,333,444}});
         cout << "mat from list of lists: " << mat.size() << "," << mat[0].size() << "," << mat[1].size() << "," << mat[2].size() << "\n";
         for (size_t i=0; i<mat.size(); ++i) {
            for (size_t j=0; j<mat[i].size(); ++j) {
               cout << std::setw(4) << mat[i][j] << " ";
            }
            cout << "\n";
         }
      }
      {
         init_from_initializer_list({{1,2,3,4}, {11,22}, {111,222,333,444,555}});
         cout << "mat from list of lists: " << mat.size() << "," << mat[0].size() << "," << mat[1].size() << "," << mat[2].size() << "\n";
         for (size_t i=0; i<mat.size(); ++i) {
            for (size_t j=0; j<mat[i].size(); ++j) {
               cout << std::setw(4) << mat[i][j] << " ";
            }
            cout << "\n";
         }
      }
      {
         // check if all inner vector call dtor when calling v.clear()
         struct S {
            S(int i) : m_val(i) { cout << "S(int i) called on " << m_val << "\n"; }
            //S(int&& i) : m_val(i) { cout << "S(int&& i) called on " << m_val << "\n"; }
            ~S() { cout << "~S() called on " << m_val << "\n"; }
            int m_val;
         };
         //std::ostream& operator<<(std::ostream& os, const S& s) { return os << s.m_val; }
         std::vector<std::vector<S>> vv {{1,2,3},{4,5,6}};
         cout << "after ctor:\n";
         vv.clear();
         
         cout << "--- print_from_init_list:\n";
         auto print_from_init_list = [](std::initializer_list<S> vals) {
            for (auto& v : vals) {
               cout << v.m_val << ", ";
            }
            cout << "\n";
         };
         print_from_init_list({1,2,3,4,5});
         cout << "---" << "\n";

         std::initializer_list<S> l = {11,22,33,44,55};
         print_from_init_list(l);
         cout << "---" << "\n";
         
         struct vec_of_S {
            vec_of_S(S s1, S s2) : m_vals({s1,s2})            { cout << "vec_of_S(S s1, S s2) called\n"; }
            vec_of_S(std::initializer_list<S> l) : m_vals(l)  { cout << "vec_of_S(std::initializer_list<S> l) called\n"; }
            vec_of_S(std::initializer_list<S>& l) : m_vals(l) { cout << "vec_of_S(std::initializer_list<S>& l) called\n"; }
            //vec_of_S(const std::initializer_list<S>& l) : m_vals(l) { cout << "const vec_of_S(std::initializer_list<S>& l) called\n"; }
            //vec_of_S(std::initializer_list<S>&& l) : m_vals(l) { cout << "vec_of_S(std::initializer_list<S>&& l) called\n"; }
            ~vec_of_S() { cout << "~vec_of_S() called\n"; }
            std::vector<S> m_vals;
         };
         cout << "--- vS:\n";
         vec_of_S vS = {1,2}; // 'vS{1,2}' == 'vS{{1,2}}' == 'vS({1,2})' == 'vS = {1,2}' != vS(1,2)
      
         // move-ctor p.21
         struct X {
            X(int n) : m_bytes(n) {
               cout << "ctor\n";
               m_data = new char[n]; }
            ~X() {
               cout << "dtor\n";
               delete[] m_data;
               m_data = nullptr;
            }
            
            X(const X& other) {
               cout << "copy-ctor\n";
               m_bytes = other.m_bytes;
               m_data = new char[other.m_bytes];
               memcpy(m_data, other.m_data, other.m_bytes);
            }
            X(X&& other) {
               cout << "move-ctor\n";
               m_bytes = other.m_bytes;
               m_data = other.m_data;
               other.m_data = nullptr; // IMPORTANT! otherwise double free!
            }

            X& operator=(const X& other) {
               cout << "copy-assignment\n";
               this->~X();
               m_bytes = other.m_bytes;
               m_data = new char[other.m_bytes];
               memcpy(m_data, other.m_data, other.m_bytes);
               return *this;
            }
            X& operator=(X&& other) {
               cout << "move-assignment\n";
               this->~X();
               m_bytes = other.m_bytes;
               m_data = other.m_data;
               other.m_data = nullptr; // IMPORTANT! otherwise double free!
               return *this;
            }

            int m_bytes;
            char* m_data;
         };
         cout << "--- struct X:\n";
         X x1(100);
         X x2 = x1;
         X x3(x1);
         X x4(std::move(x1));
         X x5(10);
         X x6(20);
         x5 = x2;
         x6 = std::move(x2);

         cout << "--- all done, dtors from this block follow...\n";
      }
   }
   print_hline();

   {
      int array[] = { 1, 2, 3, 4, 5 };
      cout << "array N_bytes,N_elements: " << sizeof(array) << "," << sizeof(array)/sizeof(array[0]) << "\n";
   }
   print_hline();

   {
      cout << "the usual way:       " << "\\\\n"  << " -- " << std::string( "\\\\n"  ).size() << "\n";
      cout << "raw string literals: " << R"(\\n)" << " -- " << std::string( R"(\\n)" ).size() << "\n";
      cout << "raw string literals: " << R"++(\\n )" )++" << "\n";
      cout << "utf-8 string:        " << u8"·–-_~€@µ" << "\n";
      cout << "char  string:        " <<   "·–-_~€@µ" << "\n";
      cout << (int)std::numeric_limits<char>::min()          << "," << (int)std::numeric_limits<char>::max()          << "\n";
      cout << (int)std::numeric_limits<char>::is_signed << "\n";
      cout << (int)std::numeric_limits<unsigned char>::min() << "," << (int)std::numeric_limits<unsigned char>::max() << "\n";
      cout << std::numeric_limits<short>::min()              << "," << std::numeric_limits<short>::max()              << "\n";
      cout << std::numeric_limits<int>::min()                << "," << std::numeric_limits<int>::max()                << "\n";
      cout << std::numeric_limits<long>::min()               << "," << std::numeric_limits<long>::max()               << "\n";
      cout << std::numeric_limits<long long>::min()          << "," << std::numeric_limits<long long>::max()          << "\n";
      cout << std::numeric_limits<unsigned long long>::min() << "," << std::numeric_limits<unsigned long long>::max() << "\n";
      cout << std::numeric_limits<nullptr_t>::min()          << "," << std::numeric_limits<nullptr_t>::max()          << "\n";

      cout << "float  eps:         " << std::numeric_limits<float>::epsilon() << "\n";
      cout << "double eps:         " << std::numeric_limits<double>::epsilon() << "\n";
      cout << "float  round_error: " << std::numeric_limits<float>::round_error() << "\n"; // TODO: checkout 'ISO/IEC 10967-1'
      cout << "double round_error: " << std::numeric_limits<double>::round_error() << "\n";
   

      enum class LogLevels {
         INFO,
         DEBUG,
         TRACE
      };
      cout << "underlying type of enum class: " << typeid(std::underlying_type<LogLevels>).name() << "\n";
      // shows 'St15underlying_typeIZ4mainE9LogLevelsE' for both default and using ': char'
   }
   print_hline();

   {
      {
         // to pass variable by ref to std::pair (same for std::tuple)
         int i = 0;
         auto p1 = std::pair<int&,int&>(i,i);
         p1.first++;
         cout << "i = " << i << "\n";

         auto p2 = std::make_pair(std::ref(i),std::ref(i));
         p2.first++;
         cout << "i = " << i << "\n";
      }
      {
         cout << "std::pair:\n";
         std::pair<int, float> p = {11, 3.14159};
         cout << p.first << " " << p.second << " | " << std::get<0>(p) << " " << std::get<1>(p) << "\n";
      }
      {
         // not possible in C++11, but since when? 14 or 17?
         std::tuple<int,double> t3 = {42,3.14};
         std::vector<std::tuple<int,float>> v { {1,1.0}, {2,2.0} };
         cout << t3 << "\n";
         for (const auto& e : v)
            cout << e << "\n";
      }
      // printing of tuple of arbitrary size
      cout << "testing printing of tuple:\n";
      std::tuple<int, float, std::string> t1(42, 1.5, "someStr");
      cout << std::get<0>(t1) << " "
           << std::get<1>(t1) << " "
           << std::get<2>(t1) << "\n";
      cout << t1 << "\n";
      std::tuple<int, double, std::string> t2{11, 3.14159, "pi"};
      cout << t2 << "\n";
      cout << "num of elements in t2: "
           << std::tuple_size<decltype(t2)>::value << "\n";
      cout << std::string(40, '~') << "\n";
   }
   print_hline();

   testing_smart_pointers();
   print_hline();

   testing_const();
   print_hline();

   testing_function_taking_Base();
   print_hline();

   testing_virtual();
   print_hline();

   testing_reference_wrapper();
   print_hline();

   testing_timing();
   print_hline();

   testing_vector_capacity();
   print_hline();

   test_operator_precedence();
   print_hline();

   testing_range_forming_iterator_positions();
   print_hline();

   testing_handling_multiple_ranges();
   print_hline();

   testing_remove_erase();
   print_hline();

   testing_transform();
   print_hline();

   testing_list_splice();
   print_hline();

   testing_forward_list();
   print_hline();

   testing_set();
   print_hline();

   testing_map();
   print_hline();

   testing_unordered_container_custom_hash();
   print_hline();

   testing_container_reference_semantics();
   print_hline();

   testing_multiple_sorting_criteria();
   print_hline();

   testing_processing_adjacent_elements();
   print_hline();

   testing_shift_left();
   print_hline();

   testing_nth_element();
   print_hline();

   testing_next_permutation();
   print_hline();

   testing_shuffle();
   print_hline();

   testing_bitset();
   print_hline();

   testing_string();
   print_hline();

   testing_stream_redirect();
   print_hline();

   HERE:
   testing_locales();
   print_hline();
   goto END;

   testing_string_conversions();
   print_hline();

   testing_random();
   print_hline();

   testing_async_future();
   print_hline();

   testing_locking();
   print_hline();

   END:
   return 0;
}