<?php


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT activity.*
                              FROM activity
                              WHERE activity.id_activity='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $shortcut_value = $edit_array[0][shortcut];
                   $name_value = $edit_array[0][name];
                   $description_value = $edit_array[0][description];
                   }



/*
                   <style scoped>
            .button-error {
                color: white;
                border-radius: 4px;
                text-shadow: 0 1px 1px rgba(0, 0, 0, 0.2);
                background: rgb(202, 60, 60); /* this is a maroon

            }
                  </style>
*/


?>



<div>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >

            <div class="pure-control-group">
                <label for="shortcut">Shortcut</label>
                <input type="text" name="shortcut" id="shortcut" value="<?php echo $shortcut_value; ?>" class="pure-input-1-4" autofocus>
            </div>
            <div class="pure-control-group">
                <label for="name">Name</label>
                <input type="text" name="name" id="name" value="<?php echo $name_value; ?>" class="pure-input-1-4">
            </div>
            <div class="pure-control-group">
                <label for="description">Description</label>
                <input type="text" name="description" id="description" value="<?php echo $description_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-controls">

                <?php
                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary">Submit</button>';

                      if($action<>'add')
                      echo '<a href="?action=del&id='.$id.'" class="pure-button button-error">Delete</a>';
                   }

                 ?>

            </div>
        </fieldset>
    </form>


</div>
