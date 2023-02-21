#pragma once

#ifdef MCC_WITH_CUDA

#include <cusolverSp.h>

extern cusolverSpHandle_t solver_handle;
extern cusparseMatDescr_t descr;
extern bool bUseCUDA;

bool cuSolverRelsease();
bool cuSolverInit();

#endif
