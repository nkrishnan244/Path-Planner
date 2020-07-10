# Path-Planner
a c++ path planner class 

The goal of this project was to take path planning algorithms that I do know (Dijkstra, A*, RRT) as well as algorithms that I have not used before (Q-Learning, D*), and practice implementing them in C++ using Object Oriented Design. Another main goal was to learn how to use Qt to make a UI to go with it. 

# Dijkstra

Dijkstra is an algorithm that makes use of a weighted graph (a graph where nodes are associated with a certain value). The value of each node is associated only with the number of steps required to reach that node from the start point. 

Status: Done

# A-Star

A* is a form of Dijkstra's algorith with a slight change. Instead of the value of each node being associated only with the number of steps, it uses a heuristic function which is defined as the distance between that vertex and and the final node position. The value of each node is the number of steps + heuristic function.

Status: Done

# D-Star

Status: Needs implementation 

# RRT 

RRT is a path planning algorithm that samples random nodes to create a tree 

Status: Done

# Q-Learning

Q learning is a reinforcement learning algorithm that explores the grid without a policy and slowly comes up with a deterministic policy based on the exploration. The reward system is the follwing:

Status: Needs clean up 
