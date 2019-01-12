<?php

$stmt = $dbh->prepare("SELECT cash_book_group.id_cash_book_group, cash_book_group.name as cb_name, cash_book_group.type,
                                  case cash_book_group.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name

                              FROM cash_book_group
                              ORDER BY cash_book_group.type, cash_book_group.name");
$stmt -> execute();
$cash_book_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT location_groups.id_location_groups, location_groups.name as loc_name
                              FROM location_groups
                              ORDER BY location_groups.name");
$stmt -> execute();
$location_array = $stmt->fetchAll();


$id = $_GET['id'];

if($action=='add')
                   {
                   $location_select = $_GET['pre_loc'];
                   $cash_book_group_select = $_GET['pre_cb'];
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

<form class="pure-form pure-form-aligned" action="?period=<?php echo $period; ?>&action=post" method="POST">
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
                        if( $location_select == $row['id_location_groups'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_location_groups'].'" '.$sel.'>'.$row['loc_name'].'</option>';
                        }
                ?>
                </select>
            </div>

            <div class="pure-control-group">
                <label for="cash_book_group_id">Group</label>
                <select name="cash_book_group_id" id="cash_book_group_id" class="pure-input-1-4">

                <?php
                     foreach($cash_book_array as $row)
                        {
                        if( $cash_book_group_select == $row['id_cash_book_group'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_cash_book_group'].'" '.$sel.'>'.$row['type_name'].': '.$row['cb_name'].'</option>';
                        }
                ?>

                </select>

                <input name="type" id="type" type="hidden" value="2">

            </div>

            <div class="pure-control-group">
                <label for="value">Cost budget</label>
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
