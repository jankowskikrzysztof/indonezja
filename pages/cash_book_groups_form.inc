<?php

$stmt = $dbh->prepare("SELECT cash_book_group.id_cash_book_group, cash_book_group.name as cb_name,
                                  case cash_book_group.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name

                              FROM cash_book_group
                              ORDER BY cash_book_group.type, cash_book_group.name");
$stmt -> execute();
//$type_array = $stmt->fetchAll();

$type_array[0] = array(type_id=>'1', type_name=>'income');
$type_array[1] = array(type_id=>'2', type_name=>'cost');

$id = $_GET['id'];


$period_value = $period_year.'-'.$period_month.'-01';

if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT cash_book_group.*,
                                  case cash_book_group.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                              FROM cash_book_group
                              WHERE cash_book_group.id_cash_book_group='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $name_value = $edit_array[0][name];
                   $type_select = $edit_array[0][type];
                   $type_name_value = $edit_array[0][type_name];
                   }

?>



<div>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >

            <div class="pure-control-group">
                <label for="name">Name</label>
                <input type="text" name="name" id="name" value="<?php echo $name_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="type">Type</label>
                <select name="type" id="type" class="pure-input-1-4">
                <?php
                     foreach($type_array as $row)
                        {
                        if( $type_select == $row['type_id'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['type_id'].'" '.$sel.'>'.$row['type_name'].'</option>';
                        }
                ?>
                </select>
            </div>

            <div class="pure-controls">


                <?php
                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary">Submit</button>';

                      if($action<>'add' and $_SESSION['user_id']==1)
                      echo '<a href="?action=del&id='.$id.'" class="pure-button button-error">Delete</a>';
                   }

                 ?>



            </div>
        </fieldset>
    </form>


</div>
