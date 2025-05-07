In UBUNTU:


*************** HPC ******************
- How to run normal C++ programs (HPC 1st and 2nd Program)?

>> g++ program_name.cpp -o program_name
>> ./program_name

- How to run HPC 3rd Program?

>> g++ -fopenmp program_name.cpp -o program_name
>> ./program_name

- How to run HPC 4th CUDA Program?

1. Open Google Colab
2. Change the Runtime to T4 GPU
3. Upload the code(s) or write in the Colab Notebook.
4. Run the cells.

*************** DL ******************
To run DL Programs, follow below steps:
1. Open Terminal
2. Type "anaconda-navigator" or "jupyter lab" or "jupyter notebook"
3. If you type "anconda-navigator", you'll see a GUI where you can select "jupyter lab" or "jupyter notebook", open anyone of them (preferably "jupyter notebook" if using in College)
4. Now, upload or write the codes.
5. Run the cells.

Note: If you face problem running code on Jupyter, just open the terminal and run below commands-
conda install -y pandas numpy matplotlib scikit-learn
pip install tensorflow

You're good to go now!!
