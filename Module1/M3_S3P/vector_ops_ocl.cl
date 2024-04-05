__kernel void vector_add_ocl(const int size, __global int *v1, __global int *v2, __global int *v_out) {
    const int globalIndex = get_global_id(0);
  
    if (globalIndex < size) {
        // Compute index for this work-item
        int idx = get_global_id(0);
        
        // Perform addition in parallel
        v_out[idx] = v1[idx] + v2[idx];
    }
}
