<?php

namespace Drupal\ldap_query\Plugin\views\field;

use Drupal\Component\Render\FormattableMarkup;
use Drupal\Core\Form\FormStateInterface;
use Drupal\views\ResultRow;

/**
 * Let's the user choose which LDAP attribute to use from the query.
 *
 * @ingroup views_field_handlers
 *
 * @ViewsField("ldap_variable_image_attribute")
 */
class LdapVariableImageAttribute extends LdapVariableAttribute {

  /**
   * Encodes a binary image for display directly in Views.
   */
  public function render(ResultRow $values) {
    $output = parent::render($values);
    if ($this->getValue($values)) {
      $data = $this->getValue($values)[0];
      $formatter = new FormattableMarkup(
        '<img src="data:image/jpeg;base64,:src"/>', [
          ':src' => base64_encode($data)
        ]
      );
      return ['#markup' => $formatter];
    }
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    parent::buildOptionsForm($form, $form_state);
    // To avoid code complexity, multi-value is removed for images, since
    // that is in unusual scenario.
    unset($form['multi_value']);
    unset($form['value_separator']);
    unset($form['index_value']);
  }

}