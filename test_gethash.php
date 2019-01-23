<form id="formLogin" method="post" name="myform">
   <label>User Name:</label>
   <input type="text" name="username" id="username">


   <input type="submit" value="Login" id="submit">
   <input type="reset" value="Reset">
</form>

<!--<script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>-->
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.10.1/jquery.min.js" type="text/javascript"></script>
<script src="http://ajax.aspnetcdn.com/ajax/jquery.validate/1.11.1/jquery.validate.min.js" type="text/javascript"></script> 

<script>

$().ready(function() {

$.validator.addMethod("checkUserName", 
    function(value, element) {
        var result = false;
        $.ajax({
            type:"POST",
            async: false,
            url: "test_hash_db.php", // script to validate in server side
            data: {username: value},
            success: function(data) {
                result = (data == true) ? true : false;
            }
        });
        // return true if username is exist in database
        return result; 
    }, 
    "This username is already taken! Try another."
);

// validate signup form on keyup and submit
$("#register-form").validate({
    rules: {
        "username": {
            required: true,
            checkUserName: true
        }
    }
});
});   

</script>