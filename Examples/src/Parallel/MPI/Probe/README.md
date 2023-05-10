# A possible use of MPI_Probe()

`MPI_Probe` obtains information about a message that is waiting for reception, **without actually receiving it**. In other words, the message probed remains waiting for reception afterwards. What it receive is a `MPI_Status` object that refers to the message to be received. The status contains, among other things, the length of the message. 

Thus, this approach allows, for instance, to receive messages of unknown length by probing them to get their length first. If there is no message from the given source with the given tag waiting for reception, MPI_Probe will block until such a message arrives.

The synopsis is 

	int MPI_Probe(int source,int tag, MPI_Comm communicator, MPI_Status* status);
	
- `source`  The rank of the sender, which can be `MPI_ANY_SOURCE` to exclude the sender's rank from message filtering.
- `tag` The tag to require from the message. If no tag is required, `MPI_ANY_TAG` can be passed.
- `communicator` The communicator concerned.
- `status` The variable in which store the status corresponding to the message probed (if any), which can be `MPI_STATUS_IGNORE` if unused (but normally it is the status you want to probe!).

Having obtained the status, if the message lenght is what you are looking for, you can use `MPI_Get_count`:

	int MPI_Get_count(const MPI_Status* status,MPI_Datatype datatype,int* count);

with which you get in `count` a number that represents the size of the message to which `status` refer, in terms of number of elements of type `datatype`.

In this simple code, the probing is indeed used to dimension a `std::vector` correctly, before receiving it from another process.

To run the code after compilation:

	mpirun -n 2 ./main_probe

since this code runs only with just 2 MPI processes (it's just an example!).

# What do I learn here?

-How the use of probing allow to correctly dimension receive data buffers of unknown length.
- How to use `MPI_Probe` and `MPI_Get_count` to obtain information about a message without actually receiving it.


