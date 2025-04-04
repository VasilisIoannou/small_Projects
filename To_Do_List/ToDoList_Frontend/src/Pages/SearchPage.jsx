import React from 'react'
import { useLocation } from 'react-router-dom';
import { useState, useEffect} from 'react';

import Card from '../Component/Card';

const SearchPage = () => {

    const location = useLocation();
    const searchData = location.state?.searchData;
    const [serachCards, setSearchCards] = useState(searchData);

    useEffect(() => {
        // Update serachCards when searchData changes
        if (searchData) {
            setSearchCards(searchData);
        }
    }, [searchData]);

    function handleCardDelete(cardId){
        setSearchCards(prevCardData => prevCardData.filter(card => card.id !== cardId));
    }

    return (
        <div>
            {searchData ? (
                serachCards.map((card) => (
                    <Card key={card.id} id={card.id} T={card.title} body={card.body} onDelete={handleCardDelete}/>
                ))  
            ) : (
                <div>No data available</div>
            )}
        </div>
    );
}

export default SearchPage