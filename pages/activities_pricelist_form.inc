<?php

$stmt = $dbh->prepare("SELECT location_groups.id_location_groups, location_groups.name as loc_name
                              FROM location_groups
                              ORDER BY location_groups.name");
$stmt -> execute();
$location_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT activity.id_activity, activity.name as act_name, activity.shortcut as act_shortcut
                              FROM activity
                              ORDER BY activity.name");
$stmt -> execute();
$activity_array = $stmt->fetchAll();


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT activity_price_list.*
                              FROM activity, activity_price_list, location_groups
                              WHERE activity.id_activity=activity_price_list.activity_id
                              and location_groups.id_location_groups=activity_price_list.location_groups_id
                              and activity_price_list.id_activity_price_list='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $activity_select = $edit_array[0][activity_id];
                   $location_select = $edit_array[0][location_groups_id];
                   $price = $edit_array[0][price];
                   }




?>



<div>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >


            <div class="pure-control-group">
                <label for="activity_id">Activity</label>
                <select name="activity_id" id="activity_id" class="pure-input-1-4">
                <?php
                     foreach($activity_array as $row)
                        {
                        if( $activity_select == $row['id_activity'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_activity'].'" '.$sel.'>'.$row[act_shortcut].': '.$row['act_name'].'</option>';
                        }
                ?>
                </select>
            </div>


            <div class="pure-control-group">
                <label for="location_groups_id">Location group</label>
                <select name="location_groups_id" id="location_groups_id" class="pure-input-1-4">
                <?php
                     foreach($location_array as $row)
                        {
                        if( $location_select == $row['id_location_groups'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_location_groups'].'" '.$sel.'>'.$row['loc_name'].'</option>';
                        }
                ?>
                </select>
            </div>


            <div class="pure-control-group">
                <label for="name">Price</label>
                <input type="text" name="price" id="price" value="<?php echo $price; ?>" class="pure-input-1-4">
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
