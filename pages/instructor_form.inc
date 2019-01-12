<?php


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT instructor.*
                              FROM instructor
                              WHERE instructor.id_instructor='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $name_value = $edit_array[0][name];
                   $percentage_value = $edit_array[0][percentage];
                   }

?>



<div>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >

            <div class="pure-control-group">
                <label for="name">Name</label>
                <input type="text" name="name" id="name" value="<?php echo $name_value; ?>" class="pure-input-1-4" autofocus>
            </div>

            <div class="pure-control-group">
                <label for="percentage">Percentage</label>
                <input type="text" name="percentage" id="percentage" value="<?php echo $percentage_value; ?>" class="pure-input-1-4"> %
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
