<?php


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT company.*
                              FROM company
                              WHERE company.id_company='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $name_value = $edit_array[0]['name'];
                   $address_value = $edit_array[0]['address'];    
                   $code_value = $edit_array[0]['code'];    
                   $city_value = $edit_array[0]['city'];    
                   $nation_value = $edit_array[0]['nation'];    
                   $identification_number_value = $edit_array[0]['identification_number'];
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
                <label for="percentage">Address</label>
                <input type="text" name="address" id="address" value="<?php echo $address_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="percentage">Code</label>
                <input type="text" name="code" id="code" value="<?php echo $code_value; ?>" class="pure-input-1-8">
            </div>

            <div class="pure-control-group">
                <label for="percentage">City</label>
                <input type="text" name="city" id="city" value="<?php echo $city_value; ?>" class="pure-input-1-8">
            </div>

            <div class="pure-control-group">
                <label for="percentage">Nation</label>
                <input type="text" name="nation" id="nation" value="<?php echo $nation_value; ?>" class="pure-input-1-8">
            </div>

            <div class="pure-control-group">
                <label for="percentage">Identification number</label>
                <input type="text" name="identification_number" id="identification_number" value="<?php echo $identification_number_value; ?>" class="pure-input-1-8">
            </div>

            <div class="pure-controls">

                <?php
                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary">Submit</button> ';

                   if($action<>'add')
                      echo '<a href="?action=del&id='.$id.'" class="pure-button button-error">Delete</a>';
                   }

                 ?>


            </div>
        </fieldset>
    </form>


</div>
