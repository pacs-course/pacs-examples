# An exampe of OpenMP locks #

A simple example that shows a possible use of locks. Two classes have a method that updates an internal variable.
In one of the classes (called `Locked`) the update is protected with a lock:


	void update_c(){
        omp_set_lock(&myLock);
        counter++;
        omp_unset_lock(&myLock);
        }
 
 while in the other (`NoLocked`) it is not:
 
 ```
 void update_c(){counter++;}
 ```

 We have a vector of objects of those classes and an array of indexes, with indexes repeated. We execute a parallel loop over the indexes and call the method `update_c` of the corresponding object.
 
 In the case of the loop over the `NoLocked` objects, the absence of the lock may cause an error in the final counting. Moreover, different executions of the program may cause different outputs.
 
 These is a typical use of the lock: make mutually exclusive the access to a class member. 
 
 Note that the `Locked` class has a constructor explicitly defined, since it initializes the lock variable member of the class.
 
# What do I learn here?
 
-A possible use of OpenMP Locks.
