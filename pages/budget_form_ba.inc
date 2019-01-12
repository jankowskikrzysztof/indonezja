<?php

$stmt = $dbh->prepare("SELECT id_activity, name, shortcut

                              FROM activity
                              ORDER BY name");
$stmt -> execute();
$activity_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT location_groups.id_location_groups, location_groups.name as loc_name
                              FROM location_groups
                              ORDER BY location_groups.name");
$stmt -> execute();
$location_array = $stmt->fetchAll();


$id = $_GET['id'];

if($action=='add_ba')
                   {
                   $location_select = $_GET['pre_loc'];
                   $activity_select = $_GET['pre_act'];
                   }


$period_value = $period_year.'-'.$period_month.'-01';

if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT budget.*
                              FROM budget
                              WHERE budget.id_budget='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $period_value = $edit_array[0][date];
                   $location_select = $edit_array[0][location_groups_id];
                   $cash_book_group_select = $edit_array[0][cash_book_group_id];
                   $value_value = $edit_array[0][value];
                   $currency_value = $edit_array[0][currency];
                   }

?>



<div>

<form class="pure-form pure-form-aligned" action="?period=<?php echo $period; ?>&action=post_ba" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >

            <div class="pure-control-group">
                <label for="period">Period</label>
                <input type="text" name="period" id="period" value="<?php echo $period_value; ?>" readonly>
            </div>

            <div class="pure-control-group">
                <label for="location_groups_id">Location group</label>
                <select name="location_groups_id" id="location_groups_id" class="pure-input-1-4">
                <?php
                     foreach($location_array as $row)
                        {
                        if( $location_select == $row['id_location_groups']) $sel='selected'; else $sel = '';
                        //if( $location_select == '' and $pre_loc == $row['id_location_groups']) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_location_groups'].'" '.$sel.'>'.$row['loc_name'].'</option>';
                        }
                ?>
                </select>
            </div>

            <div class="pure-control-group">
                <label for="activity">Group Name</label>
                <input name="activity" id="activity" type="text" value="Activity" readonly>
                <input name="cash_book_group_id" id="cash_book_group_id" type="hidden" value="14">
                <input name="type" id="type" type="hidden" value="1">

            </div>


            <div class="pure-control-group">
                <label for="activity_id">Activity</label>
                <select name="activity_id" id="activity_id" class="pure-input-1-4">

                 <?php
                     foreach($activity_array as $row)
                        {
                        if( $activity_select == $row['id_activity']) $sel='selected'; else $sel = '';
                        //if( $location_select == '' and $pre_loc == $row['id_location_groups']) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_activity'].'" '.$sel.'>'.$row['shortcut'].': '.$row['name'].'</option>';
                        }
                ?>

                </select>
            </div>

            <div class="pure-control-group">
                <label for="amount">Amount plan</label>
                <input name="amount" id="amount" type="number" value="<?php echo $amount_value; ?>" min=0 step="any">

            </div>

            <div class="pure-control-group">
                <label for="value">Income plan</label>
                <input name="value" id="value" type="text" value="<?php echo str_replace('.00','',str_replace(',','',$value_value)); ?>" class="money">
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
