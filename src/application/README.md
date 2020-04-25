Alright so this is our web application. First it needs a purpose.
1- This is kinda like a todo list. We want to have tasks that can have subtasks that can have subtasks etc.. 
Ok we support 
  GET tasks -> top level task info
  GET subtasks/id -> list of task info
  POST task
  PUT task/id
  POST subtask/id -> creates a new subtask
  POST task/id -> create a new task
  DELETE task/id -> deletes a task and all of its subtasks. 