package com.example.demo.Controller;

import com.example.demo.Model.Content;
import com.example.demo.Repository.ContentRepositoryJdbc;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import org.springframework.http.ResponseEntity;


import java.util.List;

@RestController
@RequestMapping("/api/content")
@CrossOrigin

public class ContentController {
    private final ContentRepositoryJdbc repository;

    public ContentController(ContentRepositoryJdbc repository) {
        this.repository = repository;
    }

    //FindAll,FindById,FindByTitle,Create,Delete,Filter by desciption
    @GetMapping("/")
    public List<Content> findAll(){
        return repository.getAllContent();
    }

    @GetMapping("/{id}")
    public Content findById(@PathVariable int id){
        return repository.getContentById(id);
    }

    @GetMapping("/title/{title}")
    public List<Content> findByTitle(@PathVariable String title){
        return repository.getContentByTitle(title);
    }

    @GetMapping("/body/{body}")
    public List<Content> findByBody(@PathVariable String body){
        System.out.println(body);
        return repository.getContentByBody(body);
    }

    @ResponseStatus(HttpStatus.CREATED)
    @PostMapping
    public ResponseEntity<Content> create(@RequestBody Content content){

        Integer id = repository.createContent(content.title(),content.body());
        Content createdContent = new Content(id,content.title(), content.body()); // Replace with actual retrieval if applicable

        return ResponseEntity.status(HttpStatus.CREATED).body(createdContent);
    }

    @ResponseStatus(HttpStatus.NO_CONTENT)
    @PutMapping("/update/{id}")
    public ResponseEntity<Void>  update(@RequestBody Content content,@PathVariable int id){

        if(!repository.existsById(id)) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND).build();// Return 404 if not found
        }
        if(id != content.id()) {
            return ResponseEntity.status(HttpStatus.CONFLICT).build();
        }
        repository.updateContent(id, content.title(), content.body());
        return ResponseEntity.noContent().build(); // Return 204 No Content on successful update
    }

    @DeleteMapping("/delete/{id}")
    public ResponseEntity<Void> delete(@PathVariable int id){
        if(!repository.existsById(id)) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND).build();// Return 404 if not found
        }
        repository.deleteContent(id);
        return ResponseEntity.noContent().build(); // Return 204 No Content on successful update
    }

    //MAKE QUERY FOR WORDS IN BODY,CATEGORY,TITLE (not necessarily the whole name)

}
