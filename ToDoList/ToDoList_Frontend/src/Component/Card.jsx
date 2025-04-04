import { stringify } from 'postcss';
import React, { useState } from 'react'
import { IoTrashBin } from "react-icons/io5";

const Card = ({id,T,body,onDelete}) => {


  const [todoList,setTodoList] = useState(body);
  const [newTodo,setNewTodo] = useState('');
  const [title,setTitle] = useState(T);


  function deleteTodo(index){
    const updateTodoList = todoList.filter((_,id) => id !== index);
    setTodoList(updateTodoList); 
    updateCard(updateTodoList);
  }

  function moveUp(index){
    if(index > 0 ){
      const updateTodoList = [...todoList];
      [updateTodoList[index],updateTodoList[index-1]] = [updateTodoList[index-1],updateTodoList[index]];
      setTodoList(updateTodoList);
    }
  }

  function updateCard(updatedTodoList = todoList){

    const updatedCard = {
      id: id,
      title: title,
      body: updatedTodoList
    }

    fetch('http://localhost:8080/api/content/update/'+id.toString(),{
      method: 'PUT',
      headers: {"Content-Type": "application/json" },
      body: JSON.stringify(updatedCard)
    })
  }

   

  function deleteCard(){
    fetch('http://localhost:8080/api/content/delete/'+id.toString(),{
      method: 'DELETE',
      headers: {"Content-Type": "application/json" },
    }).then(response => {
      if (response.ok) {
        console.log("Card deleted successfully");
        onDelete(id);
      } else {
        console.error("Failed to delete the card");
      }
    })
    .catch(error => {
      console.error("Error deleting card:", error);
    });
  }

  function updateTitle() {
    updateCard();
  }

  function addTodo(event){
    if(event.key === 'Enter') {
      setTodoList((previousTodoList) => {
        const updatedTodoList = [...previousTodoList, newTodo]
        updateCard(updatedTodoList);
        return updatedTodoList;
      });
      setNewTodo(''); 
    }
  }

  return (
    <div className = "flex items-center justify-center">
    <div className = "bg-white rounded shadow p-6 m-4 w-full lg:w-3/4 lg:max-w-lg justify-center">
      <div className="bg-gray-300 p-6 rounded-lg ">
          <div className='font-bold'>
           <input className="bg-gray-300 py-2 px-3 mr-4 text-grey-darker" 
              placeholder="Add Title" 
              value = { title }
              onKeyDown={updateTitle}
              onChange={(e)=> {setTitle(e.target.value); updateTitle();}}
            /> 
          </div>
          {todoList.length!=0 && 
            <ol>
              {
                todoList.map((task, index)=>
                  <li key={index}> 
                    <span className="flex items-center">
                      <button className = "hover:text-white hover:bg-teal"
                        onClick = { () => moveUp(index)}> {task} 
                      </button>
                      <button className="p-2 text-red-800 bg hover:text-white hover:bg-teal"
                          onClick={() => deleteTodo(index)} >
                          <IoTrashBin className="mr-2"/> 
                      </button>
                    </span>
                  </li>
              )}
            </ol>
          } 
          <div className="flex mt-4">
              <input className="shadow appearance-none border rounded  py-2 px-3 mr-4 text-grey-darker" placeholder="Add Todo" 
                value = { newTodo }
                onChange={(e)=> setNewTodo(e.target.value) }
                onKeyDown = {addTodo}
              />
              <button className="flex items-center flex-no-shrink p-2 text-red-800 bg hover:text-white hover:bg-teal"
                onClick={() => deleteCard()}>
                <IoTrashBin className="mr-2"/> Delete
              </button>
          </div>
          
      </div>
    </div>
    </div>
  )
}

export default Card

