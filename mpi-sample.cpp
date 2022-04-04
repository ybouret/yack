#include <stdint.h>
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

int main(int argc, char **argv )
{
	int required = MPI_THREAD_SINGLE;
	int provided = 0;
	int res = MPI_Init_thread(&argc,&argv,required,&provided);
	if(res!=MPI_SUCCESS)
	{
	}
	MPI_Finalize();
	return 0;
}
