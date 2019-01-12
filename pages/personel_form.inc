<?php


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT personel.*
                              FROM personel
                              WHERE personel.id_personel='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $name_value = $edit_array[0][name];
                   $email_value = $edit_array[0][email];
                   $login_value = $edit_array[0][login];
                   $permission_value = $edit_array[0][permission];
                   $timezone_value = $edit_array[0][timezone];
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
                <label for="email">E-Mail</label>
                <input type="email" name="email" id="email" value="<?php echo $email_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="login">Login</label>
                <input type="text" name="login" id="login" value="<?php echo $login_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="password">Password</label>
                <input type="text" name="password" id="password" value="" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="password">Permission</label>
                <select name="permission" id="permission">
                   <?php if($action=='edit') echo '<option value="'.$permission_value.'" selected>'.$permission_value.'</option>'; ?>
                   <option value="ro">ro</option>
                   <option value="rw">rw</option>
                </select>
            </div>

            <div class="pure-control-group">
                <label for="password">timezone</label>
                <select name="timezone" id="timezone">
                   <?php if($action=='edit') echo '<option value="'.$timezone_value.'" selected>'.$timezone_value.'</option>'; ?>
                   <option value="Asia/Jakarta">Asia/Makassar</option>
                   <option value="Europe/Warsaw">Europe/Warsaw</option>
                </select>
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
