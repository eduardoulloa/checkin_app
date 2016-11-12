<?php if ($tabs): ?>
<div class="tabs" style="margin-bottom:0px;">
  <?php print render($tabs); ?>
</div>
<?php endif; ?>

    <div class="row app-content">
      <div class="medium-12 columns">
        <form data-abide class="application-form">
        <div class="row">
          <div class="medium-12 columns">
            <?php print render($page['content']); ?>
          </div>
        </div>
        </form>
      </div>
    </div>
    
    <?php if($page['footer']): ?>
    <footer>
      <div class="app-form-footer"><?php render($page['footer']); ?></div>
    </footer><!-- /footer -->
    <?php endif; ?>