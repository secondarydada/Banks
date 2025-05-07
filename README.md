# HPC and Deep Learning Program Execution Guide (Ubuntu)

This guide will help you run **HPC (High-Performance Computing)** and **Deep Learning (DL)** programs on Ubuntu systems.

---

## 🚀 HPC (High-Performance Computing)

### 1️⃣ Running Standard C++ Programs (HPC 1st & 2nd Programs)

To compile and run a regular C++ program:

```bash
g++ program_name.cpp -o program_name
./program_name
```

---

### 2️⃣ Running OpenMP Programs (HPC 3rd Program)

For programs using OpenMP (parallel processing):

```bash
g++ -fopenmp program_name.cpp -o program_name
./program_name
```

---

### 3️⃣ Running CUDA Programs (HPC 4th Program)

To run CUDA programs (GPU-based parallelism):

1. Open **[Google Colab](https://colab.research.google.com/)**  
2. Go to **Runtime > Change runtime type** and select **T4 GPU** under hardware accelerator.  
3. Upload your `.cu` code or write it directly in a notebook cell.  
4. Run the notebook cells to execute the code.

---

## 🤖 Deep Learning (DL)

### Running DL Programs (Python, TensorFlow, PyTorch, etc.)

1. Open a Terminal.  
2. Start your environment using one of the following:  
   - `anaconda-navigator`  
   - `jupyter lab`  
   - `jupyter notebook`  
3. If using `anaconda-navigator`, select **Jupyter Notebook** (recommended for college use).  
4. Upload or write your DL code in the notebook.  
5. Run the cells.

---

✅ You're all set! Happy Coding!