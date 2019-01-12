<?php


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT booker.*
                              FROM booker
                              WHERE booker.id_booker='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $name_value = $edit_array[0][name];
                   $commission_value = $edit_array[0][commission];
                   $company_id_value = $edit_array[0][company_id];              
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
                <label for="percentage">Commission</label>
                <input type="text" name="commission" id="commission" value="<?php echo $commission_value; ?>" class="pure-input-1-4"> %
            </div>

            <div class="pure-control-group">
                <label for="percentage">Company ID</label>
                <input type="text" name="company_id" id="company_id" value="<?php echo $company_id_value; ?>" class="pure-input-1-8">
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
