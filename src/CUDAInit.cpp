#ifdef MCC_WITH_CUDA

#include "CUDAInit.h"
#include "cuda_runtime.h" 
cusolverSpHandle_t solver_handle = 0;
cusparseMatDescr_t descr = 0;
bool bUseCUDA = false;
bool cuSolverRelsease()
{
	{
		if (descr)
		{
			cusparseDestroyMatDescr(descr);
			descr = nullptr;
		}

		if (solver_handle)
		{
			cusolverSpDestroy(solver_handle);
			solver_handle = nullptr;
		}

		bUseCUDA = false;
	}
	return true;
}

bool cuSolverInit()
{

	if (!solver_handle)
	{
		int deviceCount;
		cudaGetDeviceCount(&deviceCount);
		if (deviceCount == 0)
		{
			bUseCUDA = false;
			return false;
		}
		cudaDeviceProp prop;
		cudaGetDeviceProperties(&prop, 0);
		if (prop.major < 2)
		{
			bUseCUDA = false;
			return false;
		}

		cusolverStatus_t cso = cusolverSpCreate(&solver_handle);
		if (cso != CUSOLVER_STATUS_SUCCESS)
		{
			cuSolverRelsease();
			bUseCUDA = false;
			return false;
		}
	}

	if (!descr)
	{
		cusparseStatus_t csp = cusparseCreateMatDescr(&descr);
		if (csp != CUSPARSE_STATUS_SUCCESS)
		{
			cuSolverRelsease();
			bUseCUDA = false;
			return false;
		}
	}
	bUseCUDA = true;
	return true;
}


#endif
