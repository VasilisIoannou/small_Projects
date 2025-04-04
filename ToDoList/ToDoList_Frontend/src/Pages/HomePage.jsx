import React, { createElement } from 'react'

import Card from '../Component/Card'

import { useState,useEffect } from 'react'
import { useOutletContext } from 'react-router-dom';

//Make Drag and Drop feature
//Add Date Option
//Fix back end for Date

const HomePage = () => {

  const[loading, setLoading] = useState(true);
  const[cardData,setCardData] = useState([]);
  const { setCreateCard } = useOutletContext();


  useEffect(()=>{ 
    const fetchCards = async () => {
      try {
        //Pass the back end http in fecth
        const res = await fetch('http://localhost:8080/api/content/');
        const data = await res.json();

        // Map content to only needed fields
        const formattedCards = data.map((content) => ({
          id: content.id,
          title: content.title,
          body: content.body,
        }));
        setCardData(formattedCards);

      } finally{
        setLoading(false);
      }
    }

    fetchCards();
  }, []);

  function handleCardDelete(cardId) {
    setCardData(prevCardData => prevCardData.filter(card => card.id !== cardId));
  }

  function createCard(){
    const newCard ={
      title: '',
      body: [],
    }

    console.log("Create Card");

    fetch('http://localhost:8080/api/content',{
      method: 'POST',
      headers: {"Content-Type": "application/json" },
      body: JSON.stringify(newCard)
    }).then(res => res.json())
    .then(data => {
      // Add the card returned from the backend (with a unique ID)
      const cardWithId = { ...newCard, id: data.id };
      setCardData((previousCardData) => [...previousCardData, cardWithId  ]);
    }) .catch(err => {
      console.error("Failed to create card:", err);
    })
  }

  // Pass the createCard function up
  useEffect(() => {
    setCreateCard(() => createCard);
    }, [setCreateCard]);

  return (
    <>
     {loading ?(
        <div> Loading... </div> //Show Loading State
      ):(
          <div>
            {cardData.map((card) => (
              <Card key={card.id} id={card.id} T={card.title} body={card.body} onDelete={handleCardDelete} />
          ))}
          </div>)}
    </>
  )
}

export default HomePage




