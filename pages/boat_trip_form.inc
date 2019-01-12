<?php


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT boat_trip.*
                              FROM boat_trip
                              WHERE boat_trip.id_boat_trip='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $date_from_value = $edit_array[0][date_from];
                   $date_to_value = $edit_array[0][date_to];
                   $txt_route_value = $edit_array[0][txt_route];

                   $price_cabin_a_value = $edit_array[0][price_cabin_a];
                   $txt_cabin_a_value = $edit_array[0][txt_cabin_a];
                   $price_cabin_b_value = $edit_array[0][price_cabin_b];
                   $txt_cabin_b_value = $edit_array[0][txt_cabin_b];
                   $price_cabin_c_value = $edit_array[0][price_cabin_c];
                   $txt_cabin_c_value = $edit_array[0][txt_cabin_c];
                   $price_cabin_d_value = $edit_array[0][price_cabin_d];
                   $txt_cabin_d_value = $edit_array[0][txt_cabin_d];
                   $price_cabin_e_value = $edit_array[0][price_cabin_e];
                   $txt_cabin_e_value = $edit_array[0][txt_cabin_e];
                   }

?>



<div>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >

            <div class="pure-control-group">
                <label for="date_from">Date from</label>
                <input type="date" name="date_from" id="date_from" value="<?php echo $date_from_value; ?>" class="pure-input-1-1" autofocus>
                <label for="date_to">Date to</label>
                <input type="date" name="date_to" id="date_to" value="<?php echo $date_to_value; ?>" class="pure-input-1-1">
            </div>

            <div class="pure-control-group">
                <label for="txt_route">Trip name</label>
                <input type="text" name="txt_route" id="txt_route" value="<?php echo $txt_route_value; ?>" class="pure-input-1-2">
            </div>

            <div class="pure-control-group">
                <label for="price_cabin_a">A - price cabin</label>
                <input type="text" name="price_cabin_a" id="price_cabin_a" value="<?php echo str_replace('.00','',str_replace(',','',$price_cabin_a_value)); ?>" class="money">
                <label for="txt_cabin_a">A - notes</label>
                <input type="text" name="txt_cabin_a" id="txt_cabin_a" value="<?php echo $txt_cabin_a_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="price_cabin_b">B - price cabin</label>
                <input type="text" name="price_cabin_b" id="price_cabin_b" value="<?php echo str_replace('.00','',str_replace(',','',$price_cabin_b_value)); ?>" class="money">
                <label for="txt_cabin_b">B - notes</label>
                <input type="text" name="txt_cabin_b" id="txt_cabin_b" value="<?php echo $txt_cabin_b_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="price_cabin_c">C - price cabin</label>
                <input type="text" name="price_cabin_c" id="price_cabin_c" value="<?php echo str_replace('.00','',str_replace(',','',$price_cabin_c_value)); ?>" class="money">
                <label for="txt_cabin_c">C - notes</label>
                <input type="text" name="txt_cabin_c" id="txt_cabin_c" value="<?php echo $txt_cabin_c_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="price_cabin_d">D - price cabin</label>
                <input type="text" name="price_cabin_d" id="price_cabin_d" value="<?php echo str_replace('.00','',str_replace(',','',$price_cabin_d_value)); ?>" class="money">
                <label for="txt_cabin_d">D - notes</label>
                <input type="text" name="txt_cabin_d" id="txt_cabin_d" value="<?php echo $txt_cabin_d_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="price_cabin_e">E - price cabin</label>
                <input type="text" name="price_cabin_e" id="price_cabin_e" value="<?php echo str_replace('.00','',str_replace(',','',$price_cabin_e_value)); ?>" class="money">
                <label for="txt_cabin_e">E - notes</label>
                <input type="text" name="txt_cabin_e" id="txt_cabin_e" value="<?php echo $txt_cabin_e_value; ?>" class="pure-input-1-4">
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
