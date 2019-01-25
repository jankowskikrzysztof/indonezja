<form method="post" id="signup">
<div class="form-group">
    <label for="email">Email address:</label>
   <input type="text" name="email" id="email">


   <input type="submit" value="Login" id="submit">
   <input type="reset" value="Reset">
   </div>
</form>


<div class="cs-error-note"></div>

<!--<script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>-->
<script src="http://code.jquery.com/jquery-2.1.4.min.js"></script> 
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-validate/1.14.0/jquery.validate.min.js"></script>

<script>
$(document).ready(function () {
    $('#signup').validate({ 
    errorLabelContainer: "#cs-error-note",
    wrapper: "li",
    rules: {
        email: {
            required: true,
                remote: {
                    url: "test_hash_db.php",
                    type: "post"
                 }
        }
    },
    messages: {
        email: {
            required: "Please enter your email address.",
            email: "Please enter a valid email address.",
            remote: "Email already in use!"
        }
    },
    submitHandler: function(form) {
                        form.submit();
                     }
    });
});
</script>