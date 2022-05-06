### Grade
Exciting but (0%)

### Comments
This is a very cool idea but there are several things that are lacking in how you are approching this. You will have no penalty for the resubmission but I need to have more information to be sure I understand what you are doing. In general I have two worries about this proposal it is lacking details and is doing a lot more than is needed.

You must resubmit but will not be penalised. I would be happy to try to find a time to meet with you to talk and help you nail down all the issues in this project.

**Leading Question**
The biggest flaw here is the unclearness of your leading question. How will you measure the performance speed up? What defines a successful optimization? More generally, your leading question *must* explicitly give a final deliverable or target goal that defines success for the project. For example, you implement a graph coloring where every node is its own color. Was this a success? The idea of looking at these optimizations and algorithms is very cool but what your final deliverable will be is unclear.

**Dataset**
The dataset is not well described -- there is no link to the ANSI test suite and multiple versions that are readily available with a quick google search. Likewise some attempt was made to describe the data format (plaintext Lisp code) but it is not clear how the parser will take this code and convert it into a syntax tree (and later a graph). At minimum, you should explain how you will decide what is and what isnt supported by your parser, a brief summary of how the parser will be implemented, and how the eventual graph will be stored. (What are nodes and what are edges? Are their weights? Direction? How will you identify all of the above?) That said, good work clearly identifying 'N' as the number of language constructs.

There are a larger set of issues here. In general describing how you are parsing and what the IR you will be working on is going to be key to the following work.

**Algorithm**

*Traversals*

First you need to have a traversal. This is fundamentaly required so that you can verify your graph loaded correctly but you don't have one.

*Algorithm 1*

Here you say you will do CSE and then say you will be doing GVN. These are related algorithms but not identical. Either is fine but you need to be much more clear on what you are trying to do and from above much more clear on what the representation you will be working with will be. That is if you graph was clearly defined it would be simpler to understand your goals here.

*Algorithm 2*

This appears to be a plan to do register allocation with graph coloring. That is a classic solution and would be ok but without a discussion of the target and IR it is impossible to understand what you are going to do. It also includes an more interesting problem of computing live ranges. In fact computing live ranges and then register allocation would be a reasonable pair of algorithms with the correct leading question.




### Regrade

Given the updates to your proposal and our emails about it you have been approved and I am glad to see you starting to work. I have given you 90 on the regrade since you are still a bit sketchy with the details of how you are mapping the data to the graph. That being said based on the information you provided I am pretty confident you understand what you are doing in this case.
